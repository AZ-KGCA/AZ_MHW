#include "AZ_MHW/Controller/AZAIController.h"
#include <UObject/ConstructorHelpers.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AIPerceptionComponent.h>
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW\CharacterComponent\AZMonsterAggroComponent.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"

AAZAIController::AAZAIController(FObjectInitializer const& object_initializer)
{
	PrimaryActorTick.bCanEverTick = true;
	sight_config_ = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
	active_move_request_id_ = FAIRequestID::InvalidRequest;
}

void AAZAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);

	// Cast and save its owner
	owner_ = Cast<AAZMonster>(GetPawn());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZAIController] Non-AZMonster is possessing an AZAIController!"));
		return;
	}
	if (!owner_->IsAValidMonster())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZAIController] Possessing AZMonster id is not valid!"));
		return;
	}
	
	// Retrieve and set properties from the owner
	SetUpProperties();
	SetUpBehaviorTree();
	SetUpPerceptionSystem();
	acceptance_radius_ = owner_->acceptance_radius_;
	
	if (IsValid(behavior_tree_))
	{
		UE_LOG(AZMonster, Log, TEXT("[AZAIController] Running a behavior tree"));
		RunBehaviorTree(behavior_tree_);
	}
	else
	{
		UE_LOG(AZMonster, Warning, TEXT("[AZAIController] Behavior tree is null"));
	}
}

void AAZAIController::BeginPlay()
{
	Super::BeginPlay();
}

void AAZAIController::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	// If active move request exists, check if reached goal
	if (owner_->IsMoving() && active_move_request_id_ != FAIRequestID::InvalidRequest)
	{
		if (GEngine)
		{
			float distance = FVector::Distance(owner_->GetActorLocation(), target_location_);
			GEngine->AddOnScreenDebugMessage(-1, delta_seconds, FColor::White, FString::Printf(TEXT("Remaining distance: %f"), distance));
		}
		if (HasReachedLocation(target_location_))
		{
			UE_LOG(AZMonster, Log, TEXT("[AZAIController] [ID %d] Goal location is reached"), (uint32)active_move_request_id_);
			const FAIMessage success_msg(UBrainComponent::AIMessage_MoveFinished, this, active_move_request_id_, FAIMessage::Success);
			FAIMessage::Send(this, success_msg);
			active_move_request_id_ = FAIRequestID::InvalidRequest;
		}
	}
}

void AAZAIController::SetUpPerceptionSystem()
{
	sight_config_->SightRadius = sight_configs_.radius;
	sight_config_->LoseSightRadius = sight_configs_.lose_radius;
	sight_config_->PeripheralVisionAngleDegrees = sight_configs_.fov;
	sight_config_->SetMaxAge(sight_configs_.max_age);
	sight_config_->AutoSuccessRangeFromLastSeenLocation = sight_configs_.auto_success_range;
	sight_config_->DetectionByAffiliation.bDetectEnemies = true;
	sight_config_->DetectionByAffiliation.bDetectEnemies = false;
	sight_config_->DetectionByAffiliation.bDetectEnemies = false;

	GetPerceptionComponent()->SetDominantSense(*sight_config_->GetSenseImplementation());
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAZAIController::OnPlayerDetected);
	GetPerceptionComponent()->ConfigureSense(*sight_config_);
}

ETeamAttitude::Type AAZAIController::GetTeamAttitudeTowards(const AActor& other_actor) const
{
	// Check if pawn
	const APawn* pawn = Cast<APawn>(&other_actor);
	if (!pawn) return ETeamAttitude::Neutral;

	// Check if AAZCharacter
	const AAZCharacter* character = Cast<AAZCharacter>(&other_actor);
	if (!character) return ETeamAttitude::Neutral;
	
	// Check if pawn implements GenericTeamAgentInterface
	const auto team_agent = Cast<IGenericTeamAgentInterface>(&other_actor);
	if (!team_agent) return ETeamAttitude::Neutral;

	// Check actor's team id
	if (owner_->GetBehaviorType() == EMonsterBehaviorType::Neutral)
	{
		return ETeamAttitude::Neutral;
	}
	else if (owner_->GetBehaviorType() == EMonsterBehaviorType::Friendly)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		if (team_agent->GetGenericTeamId() != team_id_) return ETeamAttitude::Hostile;
		else return ETeamAttitude::Neutral;
	}
}

void AAZAIController::SetUpProperties()
{
	// Set up properties
	sight_configs_ = owner_->sight_configs_;
	patrol_range_ = owner_->patrol_range_;
	patrol_delay_ = owner_->patrol_delay_;
	percept_radius_ = owner_->percept_radius_;
}

void AAZAIController::SetUpBehaviorTree()
{
	// Get assets
	behavior_tree_ = UAZGameSingleton::instance()->monster_mgr_->GetBehaviorTree(owner_->behavior_tree_filename_);
	if (!IsValid(behavior_tree_)) return;
	
	// Initialise blackboard
	UBlackboardComponent* blackboard_component = Blackboard;
	UseBlackboard(behavior_tree_->BlackboardAsset, blackboard_component);
}

FAIRequestID AAZAIController::GetNewMoveRequestID()
{
	active_move_request_id_ = next_request_id_++;
	return active_move_request_id_;
}

void AAZAIController::OnPlayerDetected(AActor* actor, FAIStimulus const stimulus) 
{
	if (owner_->IsInCombat()) return;
	if (stimulus.WasSuccessfullySensed())
	{
		SetBlackboardValueAsBool(AZBlackboardKey::is_triggered_by_sight, true);
		owner_->aggro_component_->SetBestTarget(Cast<AAZCharacter>(actor));
		owner_->EnterCombat();
	}
}

void AAZAIController::OnEnterCombat()
{
	// if active move request exists, abort it
	if (GetMoveRequestID() != FAIRequestID::InvalidRequest)
	{
		const FAIMessage fail_msg(UBrainComponent::AIMessage_MoveFinished, this, active_move_request_id_, FAIMessage::Failure);
		FAIMessage::Send(this, fail_msg);
		active_move_request_id_ = FAIRequestID::InvalidRequest;
	}
	
	AAZCharacter* target = owner_->aggro_component_->GetTargetRef();
	GetPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
	SetBlackboardValueAsObject(AZBlackboardKey::target_character, target);
}

void AAZAIController::SetBlackboardValueAsFloat(FName bb_key_name, float bb_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsFloat(bb_key_name, bb_value);
}

void AAZAIController::SetBlackboardValueAsBool(FName bb_key_name, bool bb_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsBool(bb_key_name, bb_value);
}

void AAZAIController::SetBlackboardValueAsEnum(FName bb_key_name, uint8 enum_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsEnum(bb_key_name, enum_value);
}

void AAZAIController::SetBlackboardValueAsObject(FName bb_key_name, UObject* bb_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to an invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsObject(bb_key_name, bb_value);
}

FPathFollowingRequestResult AAZAIController::MoveToLocation(const FVector& target_location)
{
	FPathFollowingRequestResult request_result;
	request_result.MoveId = GetNewMoveRequestID();
	target_location_ = target_location;
	
	if (HasReachedLocation(target_location_))
	{
		request_result.Code = EPathFollowingRequestResult::AlreadyAtGoal;
	}
	else if (owner_->IsMoving())
	{
		// If already moving, send failure message
		request_result.Code = EPathFollowingRequestResult::Failed;
	}
	else
	{
		// start move
		owner_->SetTargetAngle(owner_->GetRelativeAngleToLocation(target_location));
		owner_->SetMoveState(EMoveState::Walk);
		request_result.Code = EPathFollowingRequestResult::RequestSuccessful;		
	}
	
	return request_result;
}

bool AAZAIController::HasReachedLocation(const FVector& location) const
{
	const float distance_to_loc = FVector::Distance(owner_->GetActorLocation(), location);
	if (distance_to_loc <= acceptance_radius_) return true;
	else return false;
}
