// Copyright Team AZ. All Rights Reserved.

#include "AZMonsterPacketMgr_Client.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/CommonSource/PacketStruct/ClientPacket/AZPacket_Monster.h"


AAZMonster_Client* UAZMonsterPacketMgr_Client::GetMonster(int32 monster_serial)
{
	if (auto monster = monster_map_.Find(monster_serial))
	{
		return *monster;
	}
	else return nullptr;
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_TRANSFORM_CMD(const FCG_MONSTER_TRANSFORM_CMD& recv_msg)
{
	AAZMonster_Client* target_monster = GetMonster(recv_msg.);
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_TRANSFORM_CMD] %s, Location: %s, Rotation %s"),
		owner_->object_serial_, recv_msg.is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *recv_msg.location.ToString(), *recv_msg.rotation.ToString());

	if (recv_msg.is_forced)
	{
		owner_->SetActorLocation(recv_msg.location);
		owner_->SetActorRotation(recv_msg.rotation);
	}
	else
	{
		// TODO need interp
		owner_->SetActorLocation(recv_msg.location);
		owner_->SetActorRotation(recv_msg.rotation);
	}
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_BODY_STATE_CMD(const FCG_MONSTER_BODY_STATE_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_BODY_STATE_CMD]"), owner_->object_serial_);

	// handle in mesh component
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Head, recv_msg.head_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Back, recv_msg.back_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::LeftWing, recv_msg.left_wing_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::RightWing, recv_msg.right_wing_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Tail, recv_msg.tail_state);
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_ENTER_COMBAT_CMD(const FCG_MONSTER_ENTER_COMBAT_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_ENTER_COMBAT_CMD]"), owner_->object_serial_);
	owner_->OnEnterCombat.Broadcast();
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_ACTION_START_CMD(const FCG_MONSTER_ACTION_START_CMD& recv_msg)
{
	MonsterActionStateInfo recv_state_info = recv_msg.state_info;
	FMonsterActionStateInfo state_info = recv_state_info.Convert();
	owner_->SetActionStateInfo(state_info);
	
	// TODO Start position
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_ACTION_START_CMD] Action name: %s, Montage section: %s, Start from: %f"),
		owner_->object_serial_, *state_info.animation_name.ToString(), *state_info.montage_section_name.ToString(), recv_msg.start_position);
}

void UAZMonsterPacketMgr_Client::Receive_FCG_MONSTER_PART_CHANGE_CMD(const FCG_MONSTER_PART_CHANGE_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_PART_CHANGE_CMD], %s %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(recv_msg.body_part), *UAZUtility::EnumToString(recv_msg.change_type));
	
	switch (recv_msg.change_type)
	{
	case EMonsterBodyPartChangeType::Wound:
		{
			owner_->OnBodyPartWounded.Broadcast(recv_msg.body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Break:
		{
			owner_->OnBodyPartBroken.Broadcast(recv_msg.body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Sever:
		{
			owner_->OnBodyPartSevered.Broadcast(recv_msg.body_part);
			break;
		}
	}
}

void UAZMonsterPacketMgr_Client::Receive_FCG_PLAYER_ATTACK_HIT_CMD(const FCG_PLAYER_ATTACK_HIT_CMD& recv_msg)
{
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_PLAYER_ATTACK_HIT_CMD], Damage dealt: %d, Position: %s"),
		owner_->object_serial_, recv_msg.damage_amount, *recv_msg.hit_position.ToString());
	//TODO UI 연결
}
