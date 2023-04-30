﻿#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/CharacterComponent/AZAggroComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/BehaviorTree/AZAIDefine.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include <Perception/AIPerceptionStimuliSourceComponent.h>
#include <Perception/AISense_Sight.h>
#include <GameFrameWork/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"

AAZMonster::AAZMonster()
{
	// Set default common properties
	monster_id_ = -1;
	team_id_ = (uint8)(EObjectType::Monster);
	state_info_.hp_ratio = 1.0f;
	state_info_.stamina_ratio = 1.0f;

	// Set default objects to hit check
	hit_object_types_.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	hit_object_types_.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// Set AI Controller class
	AIControllerClass = AAZAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Create components
	aggro_component_ = CreateDefaultSubobject<UAZAggroComponent>(TEXT("AggroComponent"));
	health_component_ = CreateDefaultSubobject<UAZMonsterHealthComponent>(TEXT("HealthComponent"));

	// Set up stimulus
	SetUpStimulus();
}

void AAZMonster::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	if (!GetMesh()->DoesSocketExist("HeadSocket"))
	{
		return Super::GetActorEyesViewPoint(OutLocation, OutRotation);
	}
	else
	{
		OutLocation = GetMesh()->GetSocketLocation("HeadSocket");
		OutRotation = GetMesh()->GetSocketRotation("HeadSocket");
	}
}

void AAZMonster::SetUpStimulus()
{
	stimulus_ = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	stimulus_->RegisterForSense(TSubclassOf<UAISense_Sight>());
	stimulus_->RegisterWithPerceptionSystem();
}

void AAZMonster::SetUpDefaultProperties()
{
	SetMonsterInfo();
	SetBossInfo();
	SetActionInfo();
}

void AAZMonster::SetMonsterInfo()
{
	// Return if monster id is not set
	if (monster_id_ == -1) return;

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
	blackboard_data_filename_	= monster_info->blackboard_data_filename;
	health_component_->base_hp_	= monster_info->base_hp;
}

void AAZMonster::SetBossInfo()
{
	// Return if monster id is not set
	if (monster_id_ == -1) return;

	// Return if boss information is not found in the table
	const FBossInfo* boss_info = UAZGameSingleton::instance()->monster_mgr->GetBossInfo(monster_id_, rank_);
	if (!boss_info)
	{
		UE_LOG(AZMonster, Warning, TEXT("Boss data is not found for monster #%d"), monster_id_);
		return;
	}

	// Set properties
	boss_id_											= boss_info->boss_id;
	has_combat_transition_anim_							= boss_info->has_transition_animation;
	rage_stats_											= FBossRageStats(boss_info->rage_required_damage, boss_info->rage_duration, boss_info->rage_outgoing_damage_multiplier, boss_info->rage_incoming_damage_multiplier);
	health_component_->sever_damage_tail_				= boss_info->sever_damage_tail;
	health_component_->tenderised_damage_multiplier_	= boss_info->tenderised_damage_multiplier;
	health_component_->escape_stats_					= FBossEscapeStats(boss_info->num_allowed_escapes, boss_info->escape_health_ratios);
	health_component_->weakness_stats_					= FBossWeaknessStats(boss_info->weakness_head, boss_info->weakness_neck, boss_info->weakness_body, boss_info->weakness_tail,boss_info->weakness_wing,boss_info->weakness_leg);
	health_component_->part_break_stats_				= FBossPartBreakStats(boss_info->break_damage_body, boss_info->break_damage_head, boss_info->break_damage_wing);
	health_component_->base_stamina_					= boss_info->base_stamina;
	health_component_->tired_duration_					= boss_info->tired_duration;
}

void AAZMonster::SetActionInfo()
{
	// Return if monster id is not set
	if (monster_id_ == -1) return;

	// Return if noncombat action information is not found in the table
	const auto noncombat_action_info = UAZGameSingleton::instance()->monster_mgr->GetMonsterNonCombatActionInfo(monster_id_);
	const auto combat_action_info = UAZGameSingleton::instance()->monster_mgr->GetMonsterCombatActionInfo(monster_id_);
	
	if (!noncombat_action_info && !combat_action_info)
	{
		UE_LOG(AZMonster, Warning, TEXT("Action data is not found for monster #%d"), monster_id_);
		return;
	}

	combat_action_map_ = *combat_action_info;
	noncombat_action_map_ = *noncombat_action_info;
}

void AAZMonster::BeginPlay()
{
	Super::BeginPlay();
	SetUpDefaultProperties();
	anim_instance_ = Cast<UAZAnimInstance_Monster>(GetMesh()->GetAnimInstance());
}

void AAZMonster::EnterCombatMode()
{
	SetMoveState(EMoveState::Stop);
	if (has_combat_transition_anim_)
	{
		SetActionMode(EMonsterActionMode::Transition);
	}
	else
	{
		SetActionMode(EMonsterActionMode::Combat);
	}
}

void AAZMonster::SetActionMode(EMonsterActionMode action_mode)
{
	state_info_.action_mode = action_mode;
	AAZAIController* controller = Cast<AAZAIController>(GetController());
	controller->SetBlackboardValueAsEnum(AZBlackboardKey::action_mode, uint8(action_mode));
}

void AAZMonster::SetMoveState(EMoveState move_state)
{
	state_info_.priority_score = EMonsterActionPriority::Locomotion;
	state_info_.move_state = move_state;
	state_info_.animation_name = NAME_None;
	state_info_.montage_section_name = NAME_None;

	AAZAIController* controller = Cast<AAZAIController>(GetController());
	controller->SetBlackboardValueAsEnum(AZBlackboardKey::move_state, uint8(move_state));
	if (move_state == EMoveState::Stop) controller->StopMovement();
}

void AAZMonster::SetTargetAngle(float angle)
{
	angle = FMath::Clamp(angle, -180.0f, 180.0f);
	state_info_.target_angle = angle;
}

//TODO Cinematic states are yet to be implemented
void AAZMonster::SetActionState(int32 action_id)
{
	bool is_found = false;

	// find the action data from table according to current action mode
	switch (state_info_.action_mode)
	{
	case EMonsterActionMode::Normal:
		{
			if (const FMonsterNonCombatActionInfo* action_data = noncombat_action_map_.Find(action_id))
			{
				state_info_.animation_name = action_data->animation_name;
				state_info_.montage_section_name = action_data->montage_section_name;
				is_found = true;
			}
			break;
		}
	case EMonsterActionMode::Transition:
	case EMonsterActionMode::Combat:
		{
			if (const FMonsterCombatActionInfo* action_data = combat_action_map_.Find(action_id))
			{
				state_info_.animation_name = action_data->animation_name;
				state_info_.montage_section_name = action_data->montage_section_name;
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
	// update common data
	state_info_.priority_score = EMonsterActionPriority::Action;

	// update target angle
	float angle = GetRelativeAngleToLocation(aggro_component_->GetTargetLocation());
	SetTargetAngle(angle);
}

void AAZMonster::AnimNotify_EndOfAction()
{
	if (!is_flying_)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	SetMoveState(EMoveState::Stop);
}

void AAZMonster::AnimNotify_JumpToAnimation(FName next_animation_name, FName next_montage_section_name)
{
	// use "Montage Set Next Section" if it is a montage 

	state_info_.animation_name = next_animation_name;
	state_info_.montage_section_name = next_montage_section_name;
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
