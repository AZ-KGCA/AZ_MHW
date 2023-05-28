#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/CharacterComponent/AZMonsterAggroComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent.h"
#include "AZ_MHW/CharacterComponent/AZMonsterPacketHandlerComponent.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/CommonSource/Define/GameDefine.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "AZ_MHW/Manager/AZResourceMgr.h"

#include <Perception/AISense_Sight.h>
#include <GameFrameWork/CharacterMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <BrainComponent.h>
#include <Components/CapsuleComponent.h>

#include "Character/Player/AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"


AAZMonster::AAZMonster()
{
	PrimaryActorTick.bCanEverTick = false;
	
	// Initialise common properties
	monster_id_ = boss_id_ = -1;
	active_action_id_ = -1;
	SetGenericTeamId(static_cast<uint8>(EObjectType::Monster));

	// Set default objects to hit check
	hit_object_types_.Add(UEngineTypes::ConvertToObjectType(ECC_PLAYER));
	hit_object_types_.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	// Set AI Controller class
	AIControllerClass = AAZAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::Disabled;
		
	// Create components
	aggro_component_ = CreateDefaultSubobject<UAZMonsterAggroComponent>(TEXT("AggroComponent"));
	health_component_ = CreateDefaultSubobject<UAZMonsterHealthComponent>(TEXT("HealthComponent"));
	mesh_component_ = CreateDefaultSubobject<UAZMonsterMeshComponent>(TEXT("MeshComponent"));
	packet_handler_component_ = CreateDefaultSubobject<UAZMonsterPacketHandlerComponent>(TEXT("PacketHandlerComponent"));
}

void AAZMonster::Init(int32 monster_id, EBossRank rank)
{
	monster_id_ = monster_id;
	rank_ = rank;
	acceptance_radius_ = 300.0f;
	
	SetUpDefaultProperties();
	aggro_component_->Init();
	health_component_->Init();
	mesh_component_->Init();
	packet_handler_component_->Init();
	InitializeRuntimeValues();
}

void AAZMonster::SetMeshAndColliders()
{
	FString name = name_.ToString();
	
	// Set capsule collider
	GetCapsuleComponent()->SetCapsuleRadius(127.0f);
	GetCapsuleComponent()->SetCapsuleHalfHeight(127.0f);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AZMonsterCapsule"));
	
	// Set skeletal mesh
	FString asset_path = FString::Printf(TEXT("/Game/AZ/Monsters/%s/Meshes/SK_%s"), *name, *name);
	if(auto sk_asset = LoadObject<USkeletalMesh>(nullptr,*asset_path))
	{
		GetMesh()->SetSkeletalMesh(sk_asset);
		GetMesh()->SetRelativeLocation(FVector(73, 0, -120));
		GetMesh()->SetCollisionProfileName(TEXT("AZMonster"));
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("[AAZMonster][%d] Skeletal mesh not found!"), monster_id_);
		return;
	}

	// Set animation class
	// not working
	FString anim_path = FString::Printf(TEXT("/Game/AZ/Monsters/%s/Animations/ABP_%s"), *name, *name);
	if (UClass* anim_asset = AZResourceHelper::LoadClass<UAnimInstance>(anim_path))
	{
		GetMesh()->SetAnimInstanceClass(anim_asset);
		anim_instance_ = Cast<UAZAnimInstance_Monster>(GetMesh()->GetAnimInstance());
		anim_instance_->owner_monster_ = this;
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("[AAZMonster][%d] ABP not found!"), monster_id_);
	}
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
	SetMeshAndColliders();
}

void AAZMonster::SetMonsterInfo()
{
	if (!IsAValidMonster()) return;

	// Return if monster id is not found in the table
	const FMonsterInfo* monster_info = UAZGameSingleton::instance()->monster_mgr_->GetMonsterInfo(monster_id_);
	if (!monster_info)
	{
		UE_LOG(AZMonster, Error, TEXT("Monster data is not found for monster #%d"), monster_id_);
		return;
	}

	// Set properties
	behavior_type_				= monster_info->behavior_type;
	sight_configs_				= monster_info->sight_configs;
	patrol_range_				= monster_info->patrol_range;
	patrol_delay_				= monster_info->patrol_delay;
	percept_radius_				= monster_info->percept_radius;
	name_						= monster_info->name;
}

void AAZMonster::SetBossInfo()
{
	// Return if monster id and/or monster rank are not set
	if (!IsABoss()) return;

	// Set properties
	FBossInfo boss_info = UAZGameSingleton::instance()->monster_mgr_->GetBossInfo(monster_id_, rank_);
	boss_id_					= boss_info.boss_id;
	has_combat_transition_anim_	= boss_info.has_transition_animation;
	rage_stats_					= boss_info.rage_stats;
}

void AAZMonster::SetActionInfo()
{
	if (!IsAValidMonster()) return;

	const auto noncombat_action_info = UAZGameSingleton::instance()->monster_mgr_->GetMonsterNonCombatActionInfo(monster_id_);
	const auto combat_action_info = UAZGameSingleton::instance()->monster_mgr_->GetMonsterCombatActionInfo(monster_id_);

	if (noncombat_action_info.Num())
		noncombat_action_map_ = noncombat_action_info;
	else
		UE_LOG(AZMonster, Warning, TEXT("Non-combat action data is not found for monster #%d"), monster_id_);

	if (combat_action_info.Num())
		combat_action_map_ = combat_action_info;
	else
		UE_LOG(AZMonster, Warning, TEXT("Combat action data is not found for monster #%d"), monster_id_);
}

void AAZMonster::InitializeRuntimeValues()
{
	is_flying_ = false;
	is_enraged_ = false;
	action_state_info_.Reset();
	aggro_component_->Reset();
	health_component_->InitializeRuntimeValues();
}

void AAZMonster::BeginPlay()
{
	checkf(monster_id_ != -1, TEXT("Monster properties are not set! Init() is not called."));
	
	Super::BeginPlay();
	SpawnDefaultController();
	anim_instance_ = Cast<UAZAnimInstance_Monster>(GetMesh()->GetAnimInstance());
}

void AAZMonster::EnterCombat(AActor* combat_instigator, bool is_triggered_by_sight)
{
	if (IsInCombat()) return;

	UE_LOG(AZMonster, Log, TEXT("[AZMONSTER] Entering combat by %s"), is_triggered_by_sight ? TEXT("sight") : TEXT("damage"));
	GetController()->SetBlackboardValueAsBool(AZBlackboardKey::is_triggered_by_sight, is_triggered_by_sight);
	aggro_component_->ActivateByEnterCombat(Cast<AAZPlayer_Origin>(combat_instigator)->object_serial_);
	
	if (has_combat_transition_anim_)
		SetActionMode(EMonsterActionMode::Transition);
	else
		SetActionMode(EMonsterActionMode::Combat);

	OnEnterCombat.Broadcast();
}

void AAZMonster::SetActionMode(EMonsterActionMode action_mode)
{
	action_state_info_.action_mode = action_mode;
	GetController()->SetBlackboardValueAsEnum(AZBlackboardKey::action_mode, static_cast<uint8>(action_mode));
}

void AAZMonster::SetMoveState(EMoveState move_state)
{
	action_state_info_.priority_score = EMonsterActionPriority::Locomotion;
	action_state_info_.move_state = move_state;
	action_state_info_.animation_name = NAME_None;
	action_state_info_.montage_section_name = NAME_None;
	packet_handler_component_->Send_SC_MONSTER_ACTION_START_CMD();

	GetController()->SetBlackboardValueAsEnum(AZBlackboardKey::move_state, static_cast<uint8>(move_state));
	GetController()->SetBlackboardValueAsEnum(AZBlackboardKey::ai_state, static_cast<uint8>(ECharacterState::Locomotion));
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

void AAZMonster::SetEnraged(bool is_enraged)
{
	is_enraged_ = is_enraged;
}

void AAZMonster::SetDead()
{
	// Stop all processes
	GetController()->BrainComponent->StopLogic(TEXT("Death"));
	
	OnDeath.Broadcast();
	
	// TODO Play death animation
}

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
			if (FMonsterNonCombatActionInfo* action_data = noncombat_action_map_.Find(action_id))
			{
				action_state_info_.animation_name = action_data->animation_name;
				action_state_info_.montage_section_name = action_data->montage_section_name;
				is_found = true;
			}
			else
			{
				int i = 0;
			}
			break;
		}
	case EMonsterActionMode::Transition:
		anim_instance_->is_doing_action_ = false;
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
	GetController()->SetBlackboardValueAsEnum(AZBlackboardKey::ai_state, static_cast<uint8>(ECharacterState::Action));
	anim_instance_->is_doing_action_ = false;

	// TODO this only covers combat mode; cannot process non-combat player conscious actions
	// Update target angle
	if (IsInCombat())
	{
		float angle = GetRelativeAngleToLocation(aggro_component_->GetTargetLocation());
		SetTargetAngle(angle);
		active_action_id_ = action_id;
	}

	UE_LOG(AZMonster, Warning, TEXT("Action selected: %s, %s"), *action_state_info_.animation_name.ToString(), *action_state_info_.montage_section_name.ToString());
	packet_handler_component_->Send_SC_MONSTER_ACTION_START_CMD(0.0f);
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

bool AAZMonster::IsEnraged() const
{
	return is_enraged_;
}

void AAZMonster::AnimNotify_EndOfAction()
{
	// if the action was a transition action, finish transition
	if (action_state_info_.action_mode == EMonsterActionMode::Transition)
	{
		SetActionMode(EMonsterActionMode::Combat);
	}
	// restore movement mode
	if (!is_flying_ && GetCharacterMovement()->MovementMode == MOVE_Flying)
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	}
	// return to normal action state
	anim_instance_->is_doing_action_ = false;
	active_action_id_ = -1;
	SetMoveState(EMoveState::None);
	
	packet_handler_component_->Send_SC_MONSTER_ACTION_END_CMD();
}

void AAZMonster::AnimNotify_JumpToAnimation(FName next_animation_name, FName next_montage_section_name)
{
	action_state_info_.animation_name = next_animation_name;
	action_state_info_.montage_section_name = next_montage_section_name;
	SetTargetAngle(aggro_component_->GetAngle2DToTarget());
	anim_instance_->is_doing_action_ = false;

	packet_handler_component_->Send_SC_MONSTER_ACTION_START_CMD();
}

void AAZMonster::AnimNotify_SetMovementMode(EMovementMode movement_mode)
{
	GetCharacterMovement()->SetMovementMode(movement_mode);
}

void AAZMonster::AnimNotify_DoSphereTrace(FName socket_name, float radius, EEffectDurationType duration_type, float duration)
{
	TArray<AActor*, FDefaultAllocator> ignore_actors;
	TArray<AActor*> overlapped_actors;
	ignore_actors.Add(this);

	// Get the location to trace at
	FVector trace_start_loc = GetMesh()->GetSocketLocation(socket_name);
	if (socket_name.IsEqual(FName("LeftFootSocket")) || socket_name.IsEqual(FName("RightFootSocket")))
	{
		trace_start_loc.Z -= 100.0f;
	}

	// Do Sphere overlap
	if (duration_type == EEffectDurationType::Instant)
	{
		UKismetSystemLibrary::SphereOverlapActors(GetWorld(), trace_start_loc, radius, hit_object_types_, AAZCharacter::StaticClass(), ignore_actors, overlapped_actors);
#if WITH_EDITOR
		DrawDebugSphere(GetWorld(), trace_start_loc, radius, 24, FColor::Red, false, 5.0f, 0U, 2.f);
#endif WITH_EDITOR

		for (auto actor : overlapped_actors)
		{
			UE_LOG(AZMonster, Log, TEXT("[AAZMonster] Sphere trace overlapped %s"), *actor->GetName());
			AAZPlayer* overlapped_player = Cast<AAZPlayer>(actor);
			if (overlapped_player->GetClass()->ImplementsInterface(UAZDamageAgentInterface::StaticClass()))
			{
				DoDamage(overlapped_player, FHitResult());
			}
		}
	}
	else if (duration_type == EEffectDurationType::ForDuration)
	{
		//@TODO
	}
}

// Damage functions are handled in the health component
float AAZMonster::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info)
{
	return health_component_->ApplyDamage(damaged_actor, hit_result, attack_info);
}

AAZAIController* AAZMonster::GetController()
{
	return Cast<AAZAIController>(Super::GetController());
}

float AAZMonster::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	return health_component_->ProcessDamage(damage_instigator, hit_result, attack_info);
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

void AAZMonster::DoDamage(AActor* damaged_actor, const FHitResult hit_result)
{
	FAttackInfo* attack_info = UAZGameSingleton::instance()->monster_mgr_->GetAttackInfo(active_action_id_);
	if (attack_info)
	{
		IAZDamageAgentInterface::Execute_ApplyDamage(this, damaged_actor, hit_result, *attack_info);
	}
}

// 메시처리등 서버에서 불필요한 부분 빼기