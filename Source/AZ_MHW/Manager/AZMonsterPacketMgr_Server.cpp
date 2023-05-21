// Copyright Team AZ. All Rights Reserved.


#include "AZMonsterPacketMgr_Server.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/CommonSource/PacketStruct/ServerPacket/AZPacket_Monster.h"

AAZMonster* UAZMonsterPacketMgr_Server::GetMonster(int32 monster_serial)
{
	if (auto monster = monster_map_.Find(monster_serial))
	{
		return *monster;
	}
	else return nullptr;
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_TRANSFORM_CMD(bool is_forced)
{
	FCG_MONSTER_TRANSFORM_CMD packet(owner_->object_serial_);
	packet.location = owner_->GetActorLocation();
	packet.rotation = owner_->GetActorRotation();

	//TODO SEND
	UE_LOG(AZMonster, Log, TEXT("[#%d][Send_FCG_MONSTER_TRANSFORM_CMD] %s, Location: %s, Rotation %s"),
		owner_->object_serial_, is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *packet.location.ToString(), *packet.rotation.ToString());
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_BODY_STATE_CMD()
{
	FCG_MONSTER_BODY_STATE_CMD packet;
	TMap<EMonsterBodyPart, FBossBodyPartState>* states = owner_->health_component_->GetBodyPartStates();
	
	packet.head_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Head));
	packet.back_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Back));
	packet.left_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::LeftWing));
	packet.right_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::RightWing));
	packet.tail_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Tail));

	//TODO SEND
	UE_LOG(AZMonster, Log, TEXT("[#%d][Send_FCG_MONSTER_BODY_STATE_CMD]"), owner_->object_serial_);
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_ENTER_COMBAT_CMD()
{
	FCG_MONSTER_ENTER_COMBAT_CMD packet(owner_->object_serial_);

	//TODO SEND
	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_ENTER_COMBAT_CMD]"), owner_->object_serial_);
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_ACTION_START_CMD(float start_position)
{
	FCG_MONSTER_ACTION_START_CMD packet(owner_->object_serial_);
	packet.state_info = MonsterActionStateInfo(owner_->action_state_info_);

	UE_LOG(AZMonster, Log, TEXT("[#%d][Send_FCG_MONSTER_ACTION_START_CMD] Action name: %s, Montage section: %s, Start from: %f"),
		owner_->object_serial_, *owner_->action_state_info_.animation_name.ToString(),
		*owner_->action_state_info_.montage_section_name.ToString(), start_position);
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type)
{
	FCG_MONSTER_PART_CHANGE_CMD packet(owner_->object_serial_);
	packet.body_part = body_part;
	packet.change_type = change_type;

	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_MONSTER_PART_CHANGE_CMD], %s %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(body_part), *UAZUtility::EnumToString(change_type));
}

void UAZMonsterPacketMgr_Server::Send_FCG_PLAYER_ATTACK_HIT_CMD(int32 attacker_serial, FVector hit_pos, int32 damage_amount)
{
	FCG_PLAYER_ATTACK_HIT_CMD packet(owner_->object_serial_);
	packet.player_serial = attacker_serial;
	packet.hit_position = hit_pos;
	packet.damage_amount = damage_amount;

	UE_LOG(AZMonster, Log, TEXT("[#%d][Receive_FCG_PLAYER_ATTACK_HIT_CMD], From: %d, Damage dealt: %d, Position: %s"),
		owner_->object_serial_, attacker_serial, damage_amount, *hit_pos.ToString());
}
