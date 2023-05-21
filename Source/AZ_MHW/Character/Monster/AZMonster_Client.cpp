// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"

AAZMonster_Client::AAZMonster_Client()
{
	// Initialise common properties
	monster_id_ = -1;
	boss_id_ = -1;

	// Disable AI Controller
	AutoPossessAI = EAutoPossessAI::Disabled;
		
	// Create components
	mesh_component_ = CreateDefaultSubobject<UAZMonsterMeshComponent_Client>(TEXT("MeshComponent"));
}

void AAZMonster_Client::PreInitializeComponents()
{
	Super::PreInitializeComponents();
	
	// Return if monster id and/or monster rank are not set
	if (monster_id_ == -1 || rank_ == EBossRank::None) return;
	
	// Return if boss information is not found in the table
	const FBossInfo* boss_info = UAZGameSingleton::instance()->monster_mgr_->GetBossInfo(monster_id_, rank_);
	if (!boss_info)
	{
		UE_LOG(AZMonster, Warning, TEXT("Boss data is not found for monster #%d"), monster_id_);
		return;
	}
	boss_id_ = boss_info->boss_id;
}

void AAZMonster_Client::BeginPlay()
{
	Super::BeginPlay();
	anim_instance_ = Cast<UAZAnimInstance_Monster>(GetMesh()->GetAnimInstance());
}

bool AAZMonster_Client::IsABoss() const
{
	return monster_id_ == -1 || rank_ == EBossRank::None;
}

void AAZMonster_Client::SetActionStateInfo(FMonsterActionStateInfo action_state_info)
{
	action_state_info_ = action_state_info;
}



