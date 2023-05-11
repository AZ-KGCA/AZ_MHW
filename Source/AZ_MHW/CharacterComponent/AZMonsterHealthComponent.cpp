// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"
#include <PhysicalMaterials/PhysicalMaterial.h>

#include "CommonSource/Define/GameDefine.h"

UAZMonsterHealthComponent::UAZMonsterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UAZMonsterHealthComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterHealthComponent] Invalid owner actor!"));
	}

	// Set monster properties
	if (!owner_->IsAValidMonster()) return;
	const FMonsterInfo* monster_info = UAZGameSingleton::instance()->monster_mgr_->GetMonsterInfo(owner_->GetMonsterID());
	base_hp_ = monster_info->base_hp;

	// Set boss properties
	if (!owner_->IsABoss()) return;
	const FBossInfo* boss_info = UAZGameSingleton::instance()->monster_mgr_->GetBossInfo(owner_->GetMonsterID(), owner_->GetBossRank());

	body_part_states_.Add(EMonsterBodyPart::Head, boss_info->head_state);
	body_part_states_.Add(EMonsterBodyPart::Back, boss_info->body_state);
	body_part_states_.Add(EMonsterBodyPart::LeftWing, boss_info->wing_state);
	body_part_states_.Add(EMonsterBodyPart::RightWing, boss_info->wing_state);
	body_part_states_.Add(EMonsterBodyPart::Tail, boss_info->tail_state);
	body_part_states_.Add(EMonsterBodyPart::Leg, boss_info->leg_state);
	
	tenderised_damage_multiplier_	= boss_info->tenderised_damage_multiplier;
	escape_stats_					= boss_info->escape_stats;
	weakness_stats_					= boss_info->weakness_stats;
	base_stamina_					= boss_info->base_stamina;
	tired_duration_					= boss_info->tired_duration;

	InitializeRuntimeValues();	
}

void UAZMonsterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// Damage Interface
	owner_->OnTakeDamage.AddDynamic(this, &UAZMonsterHealthComponent::PostProcessDamage);
	owner_->OnBodyPartWounded.AddUObject(this, &UAZMonsterHealthComponent::OnBodyPartWounded);
	owner_->OnBodyPartBroken.AddUObject(this, &UAZMonsterHealthComponent::OnBodyPartBroken);
	owner_->OnBodyPartSevered.AddUObject(this, &UAZMonsterHealthComponent::OnBodyPartSevered);
}

void UAZMonsterHealthComponent::InitializeRuntimeValues()
{
	current_hp_ = base_hp_;
	current_stamina_ = base_stamina_;
	current_num_escapes_ = 0;
	body_condition_.Reset();
}

float UAZMonsterHealthComponent::GetHealthRatio() const
{
	return current_hp_ / base_hp_;
}

float UAZMonsterHealthComponent::GetStaminaRatio() const
{
	return current_stamina_ / base_stamina_;
}

bool UAZMonsterHealthComponent::IsPendingKill() const
{
	return current_hp_ <= 0;
}

float UAZMonsterHealthComponent::ApplyDamage(AActor* damaged_actor, const FHitResult& hit_result, AController* event_instigator, const FAttackInfo& attack_info)
{
	if (IsPendingKill())
	{
		return 0.f;
	}
	if (owner_->IsInRageMode())
	{
		return attack_info.base_damage * owner_->rage_stats_.outgoing_damage_multiplier;
	}
	return attack_info.base_damage;
}

float UAZMonsterHealthComponent::ProcessDamage(const FHitResult& hit_result, AController* instigator, const FAttackInfo& attack_info, float applied_damage)
{
	if (IsPendingKill()) return 0.f;

	// TODO status effects

	// Check if damage is valid
	const EMonsterBodyPart damaged_body_part = static_cast<EMonsterBodyPart>(hit_result.PhysMaterial.Get()->SurfaceType.GetValue());
	const EDamageType damage_type = attack_info.damage_type;
	if (!IsReceivedAttackValid(damage_type, damaged_body_part)) return 0.f;
	
	// Apply weakness
	int32 weakness = weakness_stats_.weakness_array[UAZUtility::EnumToByte(damaged_body_part)][UAZUtility::EnumToByte(damage_type)];
	float processed_damage = applied_damage * (weakness / 10);

	// Apply rage mode multiplier
	if (owner_->IsInRageMode()) processed_damage *= owner_->rage_stats_.incoming_damage_multiplier;
	
	// Body part specific damage processing
	ApplyDamageToBodyPart(damage_type, damaged_body_part, processed_damage);

	// General processing
	ReduceHealth(processed_damage);

	return processed_damage;
}

void UAZMonsterHealthComponent::PostProcessDamage(float total_damage, const FAttackInfo& attack_info, AController* damage_instigator)
{
	if (IsPendingKill()) return;
}

bool UAZMonsterHealthComponent::IsReceivedAttackValid(EDamageType damage_type, EMonsterBodyPart damaged_part)
{
	// Check if valid body part is hit
	if (damaged_part == EMonsterBodyPart::Default || damaged_part >= EMonsterBodyPart::Eyelid)
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterHealthComponent] Invalid body part: %s"), *UAZUtility::EnumToString(damaged_part));
		return false;
	}

	// Check if valid damage type is received
	if (damage_type > EDamageType::Ammo)
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterHealthComponent] Invalid damage type: %s"), *UAZUtility::EnumToString(damage_type));
		return false;
	}
	return true;
}

void UAZMonsterHealthComponent::ApplyDamageToBodyPart(EDamageType damage_type, EMonsterBodyPart damaged_part, float damage_amount)
{
	EMonsterBodyPart damaged_part_group = damaged_part;
	if (damaged_part == EMonsterBodyPart::Neck) damaged_part_group = EMonsterBodyPart::Head;
	else if (damaged_part == EMonsterBodyPart::Body) damaged_part_group = EMonsterBodyPart::Back;
			
	auto body_part_info = body_part_states_.Find(damaged_part_group);

	CheckBeWounded(damaged_part_group, damage_amount);
	CheckBeBroken(damaged_part_group, damage_amount);
	if (damage_type == EDamageType::Cut) CheckBeSevered(damaged_part_group, damage_amount);

	//TODO do something by damage type

	
}

void UAZMonsterHealthComponent::CheckBeWounded(EMonsterBodyPart damaged_part, float damage_amount)
{
	auto body_part_info = body_part_states_.Find(damaged_part);
	if (!body_part_info->is_woundable) return;

	body_part_info->wound_accumulated_damage += damage_amount;
	if (body_part_info->wound_required_damage <= body_part_info->wound_accumulated_damage)
	{
		owner_->OnBodyPartWounded.Broadcast(damaged_part);
	}
}

void UAZMonsterHealthComponent::OnBodyPartWounded(EMonsterBodyPart body_part)
{
	//TODO timer
	auto body_part_info = body_part_states_.Find(body_part);
	body_part_info->is_woundable = false;
	body_part_info->wound_accumulated_damage = 0;
}

void UAZMonsterHealthComponent::CheckBeBroken(EMonsterBodyPart damaged_part, float damage_amount)
{
	auto body_part_info = body_part_states_.Find(damaged_part);
	if (!body_part_info->is_breakable) return;

	body_part_info->break_accumulated_damage += damage_amount;
	if (body_part_info->break_required_damage <= body_part_info->break_accumulated_damage)
	{
		owner_->OnBodyPartBroken.Broadcast(damaged_part);
	}
}

void UAZMonsterHealthComponent::OnBodyPartBroken(EMonsterBodyPart body_part)
{
	auto body_part_info = body_part_states_.Find(body_part);
	body_part_info->is_breakable = false;
	body_part_info->break_accumulated_damage = 0;
}

void UAZMonsterHealthComponent::CheckBeSevered(EMonsterBodyPart damaged_part, float damage_amount)
{
	auto body_part_info = body_part_states_.Find(damaged_part);
	if (!body_part_info->is_severable) return;

	body_part_info->sever_accumulated_damage += damage_amount;
	if (body_part_info->sever_required_damage <= body_part_info->sever_accumulated_damage)
	{
		owner_->OnBodyPartSevered.Broadcast(damaged_part);
	}
}

void UAZMonsterHealthComponent::OnBodyPartSevered(EMonsterBodyPart body_part)
{
	auto body_part_info = body_part_states_.Find(body_part);
	body_part_info->is_severable = false;
	body_part_info->sever_accumulated_damage = 0;
}

void UAZMonsterHealthComponent::ReduceHealth(float amount)
{
	current_hp_ -= amount;
	if (current_hp_ <= 0) Kill();
}

void UAZMonsterHealthComponent::Kill()
{
	//owner_->GetCharacterMovement()->StopMovementImmediately();
	// TODO
	UE_LOG(AZMonster, Warning, TEXT("[UAZMonsterHealthComponent] Kill Called"));

	// Damage Interface
	owner_->OnTakeDamage.RemoveDynamic(this, &UAZMonsterHealthComponent::PostProcessDamage);
	
	owner_->Destroy();
}
