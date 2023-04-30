#include "AZ_MHW/Controller/AZAIController.h"
#include <UObject/ConstructorHelpers.h>
#include <BehaviorTree/BehaviorTree.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AIPerceptionComponent.h>
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"

AAZAIController::AAZAIController(FObjectInitializer const& object_initializer)
{
	sight_config_ = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
}

void AAZAIController::BeginPlay()
{
	Super::BeginPlay();
	if (behavior_tree_) RunBehaviorTree(behavior_tree_);
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

void AAZAIController::OnPlayerDetected(AActor* actor, FAIStimulus const stimulus)
{
	// Try get owner as monster
	AAZMonster* monster = Cast<AAZMonster>(GetOwner());
	if (!monster) return;

	// Do nothing if already in combat
	if (monster->state_info_.action_mode != EMonsterActionMode::Normal) return;

	if (stimulus.WasSuccessfullySensed())
	{
		// Disable sense perception and enter combat mode
		GetPerceptionComponent()->SetSenseEnabled(UAISense_Sight::StaticClass(), false);
		monster->EnterCombatMode();
	}	
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
	const AAZMonster* monster = Cast<AAZMonster>(GetOwner());
	if (monster->behavior_type_ == EMonsterBehaviorType::Neutral)
	{
		return ETeamAttitude::Neutral;
	}
	else if (monster->behavior_type_ == EMonsterBehaviorType::Friendly)
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
	// Try get owner as monster
	const AAZMonster* monster = Cast<AAZMonster>(GetOwner());
	if (!monster) return;

	// Set up properties
	sight_configs_ = monster->sight_configs_;
	patrol_range_ = monster->patrol_range_;
	patrol_delay_ = monster->patrol_delay_;
	percept_radius_ = monster->percept_radius_;
}

void AAZAIController::SetUpBehaviorTree()
{
	// Try get owner as monster
	const AAZMonster* monster = Cast<AAZMonster>(GetOwner());
	if (!monster) return;

	// Get assets
	behavior_tree_ = UAZGameSingleton::instance()->monster_mgr->GetBehaviorTree(monster->behavior_tree_filename_);
	if (!behavior_tree_) return;

	// Initialise blackboard
	behavior_tree_->BlackboardAsset = UAZGameSingleton::instance()->monster_mgr->GetBlackboardData(monster->blackboard_data_filename_);
	Blackboard->InitializeBlackboard(*behavior_tree_->BlackboardAsset);
}

void AAZAIController::OnPossess(APawn* const pawn)
{
	Super::OnPossess(pawn);
	SetUpProperties();
	SetUpBehaviorTree();
	SetUpPerceptionSystem();
}

void AAZAIController::SetBlackboardValueAsFloat(FName bb_key_name, float bb_key_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsFloat(bb_key_name, bb_key_value);
}

void AAZAIController::SetBlackboardValueAsBool(FName bb_key_name, bool bb_key_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsBool(bb_key_name, bb_key_value);
}

void AAZAIController::SetBlackboardValueAsEnum(FName bb_key_name, uint8 enum_value)
{
	if (GetBlackboardComponent()->GetKeyID(bb_key_name) == FBlackboard::InvalidKey)
	{
		UE_LOG(AZMonster, Error, TEXT("Trying to set value to invalid blackboard key: %s"), *bb_key_name.ToString())
	}
	else GetBlackboardComponent()->SetValueAsEnum(bb_key_name, enum_value);
}
