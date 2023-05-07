#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/CharacterComponent/AZMonsterAggroComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AISense_Sight.h>
#include <GameFrameWork/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"

AAZMonster::AAZMonster()
{
	// Initialise common properties
	monster_id_ = -1;
	boss_id_ = -1;
	team_id_ = (uint8)(EObjectType::Monster);

    // TODO 
	// Set default objects to hit check
	hit_object_types_.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1)); // AZCharacter (player)
	hit_object_types_.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// Set AI Controller class
	AIControllerClass = AAZAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::Disabled;
		
	// Create components
	aggro_component_ = CreateDefaultSubobject<UAZMonsterAggroComponent>(TEXT("AggroComponent"));
	health_component_ = CreateDefaultSubobject<UAZMonsterHealthComponent>(TEXT("HealthComponent"));
	mesh_component_ = CreateDefaultSubobject<UAZMonsterMeshComponent>(TEXT("MeshComponent"));
	
	// Set up stimulus
	//SetUpStimulus();
}

void AAZMonster::SetUpStimulus()
{
	//stimulus_component_ = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus Component"));
	//stimulus_component_->RegisterForSense(TSubclassOf<UAISense_Sight>());
	//stimulus_component_->RegisterWithPerceptionSystem();
}

void AAZMonster::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	SetUpDefaultProperties();
}

void AAZMonster::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	InitializeRuntimeValues();
}

void AAZMonster::GetActorEyesViewPoint(FVector& out_location, FRotator& out_rotation) const
{
	if (!GetMesh()->DoesSocketExist("HeadSocket"))
	{
		return Super::GetActorEyesViewPoint(out_location, out_rotation);
	}
	else
	{
		out_location = GetMesh()->GetSocketLocation("HeadSocket");
		out_rotation = GetMesh()->GetSocketRotation("HeadSocket");
	}
}

void AAZMonster::SetUpDefaultProperties()
{
	SetMonsterInfo();
	SetBossInfo();
	SetActionInfo();
}

void AAZMonster::SetMonsterInfo()
{
	if (!IsAValidMonster()) return;

	// Return if monster id is not found in the table
	const FMonsterInfo* monster_info = UAZGameSingleton::instance()->monster_mgr->GetMonsterInfo(monster_id_);
	if (!monster_info)
	{
		UE_LOG(AZMonster, Error, TEXT("Monster data is not found for monster #%d"), monster_id_);
		return;
	}

	// Set properties
	behavior_type_				= monster_info->behavior_type;
	sight_configs_				= FMonsterSightConfigs(monster_info->sight_radius, monster_info->sight_lose_radius, monster_info->sight_fov, monster_info->sight_max_age, monster_info->sight_auto_success_range);
	patrol_range_				= monster_info->patrol_range;
	patrol_delay_				= monster_info->patrol_delay;
	percept_radius_				= monster_info->percept_radius;
	behavior_tree_filename_		= monster_info->behavior_tree_filename;
}

void AAZMonster::SetBossInfo()
{
	// Return if monster id and/or monster rank are not set
	if (!IsABoss()) return;
	
	// Return if boss information is not found in the table
	const FBossInfo* boss_info = UAZGameSingleton::instance()->monster_mgr->GetBossInfo(monster_id_, rank_);
	if (!boss_info)
	{
		UE_LOG(AZMonster, Warning, TEXT("Boss data is not found for monster #%d"), monster_id_);
		return;
	}

	// Set properties
	boss_id_					= boss_info->boss_id;
	has_combat_transition_anim_	= boss_info->has_transition_animation;
	rage_stats_					= FBossRageStats(boss_info->rage_required_damage, boss_info->rage_duration, boss_info->rage_outgoing_damage_multiplier, boss_info->rage_incoming_damage_multiplier);
}

void AAZMonster::SetActionInfo()
{
	if (!IsAValidMonster()) return;

	const auto noncombat_action_info = UAZGameSingleton::instance()->monster_mgr->GetMonsterNonCombatActionInfo(monster_id_);
	const auto combat_action_info = UAZGameSingleton::instance()->monster_mgr->GetMonsterCombatActionInfo(monster_id_);

	if (noncombat_action_info)
		noncombat_action_map_ = *noncombat_action_info;
	else
		UE_LOG(AZMonster, Warning, TEXT("Non-combat action data is not found for monster #%d"), monster_id_);

	if (combat_action_info)
		combat_action_map_ = *combat_action_info;
	else
		UE_LOG(AZMonster, Warning, TEXT("Combat action data is not found for monster #%d"), monster_id_);
}

void AAZMonster::InitializeRuntimeValues()
{
	is_flying_ = false;
	is_in_ragemode_ = false;
	action_state_info_.Reset();
	//TODO aggro_component_->Reset();
	health_component_->InitializeRuntimeValues();
	mesh_component_->InitializeRuntimeValues();
}

void AAZMonster::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
	anim_instance_ = Cast<UAZAnimInstance_Monster>(GetMesh()->GetAnimInstance());
}

void AAZMonster::EnterCombat()
{
	SetMoveState(EMoveState::StopMove);	
	if (has_combat_transition_anim_)
		SetActionMode(EMonsterActionMode::Transition);
	else
		SetActionMode(EMonsterActionMode::Combat);
	Cast<AAZAIController>(GetController())->OnEnterCombat();
}

void AAZMonster::SetActionMode(EMonsterActionMode action_mode)
{
	action_state_info_.action_mode = action_mode;
	AAZAIController* controller = Cast<AAZAIController>(GetController());
	controller->SetBlackboardValueAsEnum(AZBlackboardKey::action_mode, uint8(action_mode));
}

void AAZMonster::SetMoveState(EMoveState move_state)
{
	action_state_info_.priority_score = EMonsterActionPriority::Locomotion;
	action_state_info_.move_state = move_state;
	action_state_info_.animation_name = NAME_None;
	action_state_info_.montage_section_name = NAME_None;

	AAZAIController* controller = Cast<AAZAIController>(GetController());
	controller->SetBlackboardValueAsEnum(AZBlackboardKey::move_state, uint8(move_state));
	controller->SetBlackboardValueAsEnum(AZBlackboardKey::ai_state, uint8(ECharacterState::Locomotion));
	//if (move_state == EMoveState::StopMove) controller->StopMovement();
}

void AAZMonster::SetTargetAngle(float angle)
{
	angle = FMath::Clamp(angle, -180.0f, 180.0f);
	action_state_info_.target_angle = angle;
}

void AAZMonster::ResetTargetAngle()
{
	action_state_info_.target_angle = 0.0f;
}

//TODO Cinematic states are yet to be implemented
void AAZMonster::SetActionState(int32 action_id)
{
	// if current action has higher priority than next action, do not update
	if (action_state_info_.priority_score > EMonsterActionPriority::Action)
	{
		return;
	}
	bool is_found = false;

	// find the action data from table according to current action mode
	switch (action_state_info_.action_mode)
	{
	case EMonsterActionMode::Normal:
		{
			if (const FMonsterNonCombatActionInfo* action_data = noncombat_action_map_.Find(action_id))
			{
				action_state_info_.animation_name = action_data->animation_name;
				action_state_info_.montage_section_name = action_data->montage_section_name;
				is_found = true;
			}
			break;
		}
	case EMonsterActionMode::Transition:
	case EMonsterActionMode::Combat:
		{
			if (const FMonsterCombatActionInfo* action_data = combat_action_map_.Find(action_id))
			{
				action_state_info_.animation_name = action_data->animation_name;
				action_state_info_.montage_section_name = action_data->montage_section_name;
				is_found = true;
			}
			break;
		}
	default:
		{
			//@TODO Cinematic
			return;
		}
	}
	
	// return if no matching action is found
	if (!is_found)
	{
		UE_LOG(AZMonster, Error, TEXT("Failed to set action #%d"), action_id);
		return;
	}
	
	// Update state if there is an available action
	// Update common data
	action_state_info_.priority_score = EMonsterActionPriority::Action;
	Cast<AAZAIController>(GetController())->SetBlackboardValueAsEnum(AZBlackboardKey::ai_state, uint8(ECharacterState::Action));

	// TODO this only covers combat mode; cannot process non-combat player conscious actions
	// Update target angle
	if (IsInCombat())
	{
		float angle = GetRelativeAngleToLocation(aggro_component_->GetTargetLocation());
		SetTargetAngle(angle);	
	}
}

int32 AAZMonster::GetMonsterID() const
{
	return monster_id_;
}

EBossRank AAZMonster::GetBossRank() const
{
	return rank_;
}

EMonsterBehaviorType AAZMonster::GetBehaviorType() const
{
	return behavior_type_;
}

bool AAZMonster::IsInCombat() const
{
	return action_state_info_.action_mode != EMonsterActionMode::Normal;
}

bool AAZMonster::IsFlying() const
{
	return is_flying_;
}

bool AAZMonster::IsMoving() const
{
	return (action_state_info_.priority_score == EMonsterActionPriority::Locomotion
		&& action_state_info_.move_state >= EMoveState::Walk);
}

void AAZMonster::AnimNotify_EndOfAction()
{
	if (!is_flying_)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	anim_instance_->is_doing_action_ = false;
	SetMoveState(EMoveState::None);
}

void AAZMonster::AnimNotify_JumpToAnimation(FName next_animation_name, FName next_montage_section_name)
{
	// TODO use "Montage Set Next Section" if it is a montage
	action_state_info_.animation_name = next_animation_name;
	action_state_info_.montage_section_name = next_montage_section_name;
	anim_instance_->is_doing_action_ = false;
}

void AAZMonster::AnimNotify_SetMovementMode(EMovementMode movement_mode)
{
	GetCharacterMovement()->SetMovementMode(movement_mode);
}

void AAZMonster::AnimNotify_DoSphereTrace(FName socket_name, float radius, EEffectDurationType duration_type, float duration)
{
	TArray<FHitResult> hit_results;
	TArray<AActor*, FDefaultAllocator> ignore_actors;

	// Get the location to start trace at
	FVector trace_location = GetMesh()->GetSocketLocation(socket_name);

	// Do Sphere trace
	if (duration_type == EEffectDurationType::Instant)
	{
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), trace_location, trace_location, radius, hit_object_types_, false,
			ignore_actors, EDrawDebugTrace::ForDuration, hit_results, true);

		//@TODO do something with hit_results
		FString text_string = "Hit Actors: ";
		for (FHitResult hit_result : hit_results)
		{
			text_string += UKismetSystemLibrary::GetObjectName(hit_result.GetActor());
			text_string += ", ";
		}
		UE_LOG(AZMonster, Log, TEXT("%s"), *text_string);

		//@TODO get current action and handle damage

	}
	else if (duration_type == EEffectDurationType::ForDuration)
	{
		//@TODO
	}
}

bool AAZMonster::IsABoss() const
{
	if (!IsAValidMonster())
		return false;
	else
		return (rank_ != EBossRank::None);
}

bool AAZMonster::IsAValidMonster() const
{
	return (monster_id_ != -1);
}
