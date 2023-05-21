// Copyright Team AZ. All Rights Reserved.

#include "AZMonsterPacketMgr_Client.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/CommonSource/PacketStruct/ClientPacket/AZPacket_Monster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"


AAZMonster_Client* UAZMonsterPacketMgr_Client::GetMonster(int32 monster_serial)
{
	if (auto monster = monster_map_.Find(monster_serial))
	{
		return *monster;
	}
	else
	{
		UE_LOG(AZMonster_Network, Error, TEXT("[UAZMonsterPacketMgr_Client][#%d] Monster not found!"), monster_serial);
		return nullptr;
	}
}

void UAZMonsterPacketMgr_Client::AddMonster(int32 monster_serial, AAZMonster_Client* monster)
{
	if (monster_map_.Find(monster_serial))
	{
		UE_LOG(AZMonster_Network, Warning, TEXT("[UAZMonsterPacketMgr_Client][%d] Monster already exists in the monster map!"), monster_serial);
	}
	else
	{
		monster_map_.Add(monster_serial, monster);
		UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][%d] Monster added to the monster map"), monster_serial);
	}
}

// 포인터 풀릴 것 같은데 안풀리는지 체크 TODO
void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_SPAWN_CMD(const FCG_MONSTER_SPAWN_CMD& recv_msg)
{
	// Retrieve the blueprint path of the monster to spawn
	FName monster_name = UAZGameSingleton::instance()->monster_mgr_->GetMonsterName(recv_msg.monster_id);
	if (monster_name == NAME_None) return;
	FString bp_path = FString::Printf(TEXT("/Script/Engine.Blueprint'/Game/AZ/Monsters/%s/Blueprints/BP_%s.BP_%s'"),
		*monster_name.ToString(), *monster_name.ToString(), *monster_name.ToString());
	
	// Spawn a monster from the blueprint with the received transform
	UBlueprint* monster_bp = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), this, *bp_path));
	AAZMonster_Client* monster = GetWorld()->SpawnActor<AAZMonster_Client>(monster_bp->GeneratedClass, recv_msg.location, recv_msg.rotation);
	
	AddMonster(recv_msg.monster_serial, monster);
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][#%d][Receive_FCG_MONSTER_SPAWN_CMD] ID: %d, Location: %s, Rotation %s"),
		recv_msg.monster_serial, recv_msg.monster_id, *recv_msg.location.ToString(), *recv_msg.rotation.ToString());
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_TRANSFORM_CMD(const FCG_MONSTER_TRANSFORM_CMD& recv_msg)
{
	AAZMonster_Client* target_monster = GetMonster(recv_msg.monster_serial);
	if (!target_monster) return;

	if (recv_msg.is_forced)
	{
		target_monster->SetActorLocation(recv_msg.location);
		target_monster->SetActorRotation(recv_msg.rotation);
	}
	else
	{
		// TODO need interp
		target_monster->SetActorLocation(recv_msg.location);
		target_monster->SetActorRotation(recv_msg.rotation);
	}

	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][#%d][Receive_FCG_MONSTER_TRANSFORM_CMD] %s, Location: %s, Rotation %s"),
		recv_msg.monster_serial, recv_msg.is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *recv_msg.location.ToString(), *recv_msg.rotation.ToString());
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_BODY_STATE_CMD(const FCG_MONSTER_BODY_STATE_CMD& recv_msg)
{
	AAZMonster_Client* target_monster = GetMonster(recv_msg.monster_serial);
	if (!target_monster) return;

	// handle in mesh component
	target_monster->mesh_component_->SetBodyState(EMonsterBodyPart::Head, recv_msg.head_state);
	target_monster->mesh_component_->SetBodyState(EMonsterBodyPart::Back, recv_msg.back_state);
	target_monster->mesh_component_->SetBodyState(EMonsterBodyPart::LeftWing, recv_msg.left_wing_state);
	target_monster->mesh_component_->SetBodyState(EMonsterBodyPart::RightWing, recv_msg.right_wing_state);
	target_monster->mesh_component_->SetBodyState(EMonsterBodyPart::Tail, recv_msg.tail_state);

	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][#%d][Receive_FCG_MONSTER_BODY_STATE_CMD]"), recv_msg.monster_serial);
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_ENTER_COMBAT_CMD(const FCG_MONSTER_ENTER_COMBAT_CMD& recv_msg)
{
	AAZMonster_Client* target_monster = GetMonster(recv_msg.monster_serial);
	if (!target_monster) return;
	
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][#%d][Receive_FCG_MONSTER_ENTER_COMBAT_CMD]"), target_monster->object_serial_);
	target_monster->OnEnterCombat.Broadcast();
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_ACTION_START_CMD(const FCG_MONSTER_ACTION_START_CMD& recv_msg)
{
	AAZMonster_Client* target_monster = GetMonster(recv_msg.monster_serial);
	if (!target_monster) return;
	
	MonsterActionStateInfo recv_state_info = recv_msg.state_info;
	FMonsterActionStateInfo state_info = recv_state_info.Convert();
	target_monster->SetActionStateInfo(state_info);
	
	// TODO Start position
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][#%d][Receive_FCG_MONSTER_ACTION_START_CMD] Action name: %s, Montage section: %s, Start from: %f"),
		target_monster->object_serial_, *state_info.animation_name.ToString(), *state_info.montage_section_name.ToString(), recv_msg.start_position);
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_PART_CHANGE_CMD(const FCG_MONSTER_PART_CHANGE_CMD& recv_msg)
{
	AAZMonster_Client* target_monster = GetMonster(recv_msg.monster_serial);
	if (!target_monster) return;
	
	switch (recv_msg.change_type)
	{
	case EMonsterBodyPartChangeType::Wound:
		{
			target_monster->OnBodyPartWounded.Broadcast(recv_msg.body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Break:
		{
			target_monster->OnBodyPartBroken.Broadcast(recv_msg.body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Sever:
		{
			target_monster->OnBodyPartSevered.Broadcast(recv_msg.body_part);
			break;
		}
	}

	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][#%d][Receive_FCG_MONSTER_PART_CHANGE_CMD], %s %s"),
		target_monster->object_serial_, *UAZUtility::EnumToString(recv_msg.body_part), *UAZUtility::EnumToString(recv_msg.change_type));
}

void UAZMonsterPacketMgr_Client::Receive_FCG_PLAYER_ATTACK_HIT_CMD(const FCG_PLAYER_ATTACK_HIT_CMD& recv_msg)
{
	AAZMonster_Client* target_monster = GetMonster(recv_msg.monster_serial);
	if (!target_monster) return;
	
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Client][#%d][Receive_FCG_PLAYER_ATTACK_HIT_CMD], Damage dealt: %d, Position: %s"),
		target_monster->object_serial_, recv_msg.damage_amount, *recv_msg.hit_position.ToString());
	//TODO UI 연결
}
