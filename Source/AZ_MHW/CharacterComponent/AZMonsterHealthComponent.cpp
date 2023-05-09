// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Util/AZUtility.h"

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
	body_part_states_.Add(EMonsterBodyPart::Body, boss_info->body_state);
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

// void UAZMonsterHealthComponent::TakeDamage(AActor* damaged_actor, float damage_amount, const UDamageType* damage_type,
//                                            AController* event_instigator, AActor* damage_causer, FName physics_material)
// {
// 	UE_LOG(AZMonster, Log, TEXT("Damaged Actor: %s, Damage Amount: %f, Damage Type: %s, Event Instigator: %s, Damage Causer: %s"),
// 		*damaged_actor->GetName(), damage_amount, *damage_type->GetName(), *event_instigator->GetName(), *damage_causer->GetName());
// }
