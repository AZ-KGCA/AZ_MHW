// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/Character/Monster/AZMonster_Client.h"

#include "AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/CommonSource/ProtocolStruct/AZProtocol_Monster.h"

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

void AAZMonster_Client::Receive_FCG_MONSTER_TRANSFORM_CMD(const FCG_MONSTER_TRANSFORM_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_TRANSFORM_CMD] %s, Location: %s, Rotation %s"),
		object_serial_, recv_msg.is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *recv_msg.location.ToString(), *recv_msg.rotation.ToString());

	if (recv_msg.is_forced)
	{
		SetActorLocation(recv_msg.location);
		SetActorRotation(recv_msg.rotation);
	}
	else
	{
		// TODO need interp
		SetActorLocation(recv_msg.location);
		SetActorRotation(recv_msg.rotation);
	}
}

void AAZMonster_Client::Receive_FCG_MONSTER_BODY_STATE_CMD(const FCG_MONSTER_BODY_STATE_CMD& recv_msg)
{
	// handle mesh initialisation packet in the mesh component
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_BODY_STATE_CMD]"), object_serial_);
	mesh_component_->On_FCG_MONSTER_BODY_STATE_CMD(recv_msg.body_state);
}

void AAZMonster_Client::Receive_FCG_MONSTER_ENTER_COMBAT_CMD(const FCG_MONSTER_ENTER_COMBAT_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_ENTER_COMBAT_CMD]"), object_serial_);
	OnEnterCombat.Broadcast();
}

void AAZMonster_Client::Receive_FCG_MONSTER_ACTION_START_CMD(const FCG_MONSTER_ACTION_START_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_ACTION_START_CMD] Action name: %s, Montage section: %s, Start from: %f"),
		object_serial_, *recv_msg.state_info.animation_name.ToString(), *recv_msg.state_info.montage_section_name.ToString(), recv_msg.start_position);

	action_state_info_ = recv_msg.state_info;
	// TODO Start position
}

void AAZMonster_Client::Receive_FCG_MONSTER_PART_CHANGE_CMD(const FCG_MONSTER_PART_CHANGE_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_PART_CHANGE_CMD], %s %s"),
		object_serial_, *UAZUtility::EnumToString(recv_msg.body_part), *UAZUtility::EnumToString(recv_msg.change_type));
	
	switch (recv_msg.change_type)
	{
	case EMonsterBodyPartChangeType::Wound:
		{
			OnBodyPartWounded.Broadcast(recv_msg.body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Break:
		{
			OnBodyPartBroken.Broadcast(recv_msg.body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Sever:
		{
			OnBodyPartSevered.Broadcast(recv_msg.body_part);
			break;
		}
	}
}

void AAZMonster_Client::Receive_FCG_PLAYER_ATTACK_HIT_CMD(const FCG_PLAYER_ATTACK_HIT_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_PLAYER_ATTACK_HIT_CMD], Damage dealt: %d, Position: %s"),
		object_serial_, recv_msg.damage_amount, *recv_msg.hit_position.ToString());
	//TODO UI 연결
}



