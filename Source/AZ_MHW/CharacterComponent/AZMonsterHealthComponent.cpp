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
	if (owner_->monster_id_ == -1) return;
	const FMonsterInfo* monster_info = UAZGameSingleton::instance()->monster_mgr->GetMonsterInfo(owner_->monster_id_);
	base_hp_ = monster_info->base_hp;

	// Set boss properties
	if (owner_->boss_id_ == -1) return;
	const FBossInfo* boss_info = UAZGameSingleton::instance()->monster_mgr->GetBossInfo(owner_->monster_id_, owner_->rank_);
	sever_damage_tail_				= boss_info->sever_damage_tail;
	tenderised_damage_multiplier_	= boss_info->tenderised_damage_multiplier;
	escape_stats_					= FBossEscapeStats(boss_info->num_allowed_escapes, boss_info->escape_health_ratios);
	weakness_stats_					= FBossWeaknessStats(boss_info->weakness_head, boss_info->weakness_neck, boss_info->weakness_body, boss_info->weakness_tail,boss_info->weakness_wing,boss_info->weakness_leg);
	part_break_stats_				= FBossPartBreakStats(boss_info->break_damage_body, boss_info->break_damage_head, boss_info->break_damage_wing);
	base_stamina_					= boss_info->base_stamina;
	tired_duration_					= boss_info->tired_duration;

	InitializeRuntimeValues();
}

void UAZMonsterHealthComponent::InitializeRuntimeValues()
{
	current_hp_ = base_hp_;
	current_stamina_ = base_stamina_;
	current_num_escapes_ = escape_stats_.num_allowed_escapes;
}

void UAZMonsterHealthComponent::TakeDamage(AActor* damaged_actor, float damage_amount, const UDamageType* damage_type,
                                           AController* event_instigator, AActor* damage_causer)
{
}
