// Copyright Team AZ. All Rights Reserved.


#include "AZMonsterPacketMgr_Server.h"
#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/CommonSource/PacketStruct/ServerPacket/AZPacket_Monster.h"
#include "Kismet/GameplayStatics.h"

AAZMonster* UAZMonsterPacketMgr_Server::GetMonster(int32 monster_serial)
{
	if (auto monster = monster_map_.Find(monster_serial))
	{
		return *monster;
	}
	else
	{
		UE_LOG(AZMonster_Network, Error, TEXT("[UAZMonsterPacketMgr_Server][#%d] Monster not found!"), monster_serial);
		return nullptr;
	}
}

void UAZMonsterPacketMgr_Server::AddMonster(AAZMonster* monster)
{
	monster->object_serial_ = next_monster_serial_;
	monster_map_.Add(next_monster_serial_, monster);
	next_monster_serial_++;
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_SPAWN_CMD(int32 monster_serial)
{
	// must be called after AddMonster()
	AAZMonster* target = GetMonster(monster_serial);
	if (!target) return;

	FCG_MONSTER_SPAWN_CMD packet(target->object_serial_);
	packet.monster_id = target->monster_id_;
	packet.location = target->GetActorLocation();
	packet.rotation = target->GetActorRotation();

	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Server][#%d][Send_FCG_MONSTER_SPAWN_CMD] ID: %d, Location: %s, Rotation %s"),
		target->object_serial_, packet.monster_id, *packet.location.ToString(), *packet.rotation.ToString());
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_TRANSFORM_CMD(int32 monster_serial, bool is_forced)
{
	AAZMonster* target = GetMonster(monster_serial);
	if (!target) return;
	
	FCG_MONSTER_TRANSFORM_CMD packet(target->object_serial_);
	packet.location = target->GetActorLocation();
	packet.rotation = target->GetActorRotation();
	packet.is_forced = is_forced;

	//TODO SEND
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Server][#%d][Send_FCG_MONSTER_TRANSFORM_CMD] %s, Location: %s, Rotation %s"),
		target->object_serial_, is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *packet.location.ToString(), *packet.rotation.ToString());
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_BODY_STATE_CMD(int32 monster_serial)
{
	AAZMonster* target = GetMonster(monster_serial);
	if (!target) return;
	
	FCG_MONSTER_BODY_STATE_CMD packet;
	TMap<EMonsterBodyPart, FBossBodyPartState>* states = target->health_component_->GetBodyPartStates();
	
	packet.head_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Head));
	packet.back_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Back));
	packet.left_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::LeftWing));
	packet.right_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::RightWing));
	packet.tail_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Tail));

	//TODO SEND
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Server][#%d][Send_FCG_MONSTER_BODY_STATE_CMD]"), target->object_serial_);
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_ENTER_COMBAT_CMD(int32 monster_serial)
{
	AAZMonster* target = GetMonster(monster_serial);
	if (!target) return;
	FCG_MONSTER_ENTER_COMBAT_CMD packet(monster_serial);

	//TODO SEND
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Server][#%d][Receive_FCG_MONSTER_ENTER_COMBAT_CMD]"), monster_serial);
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_ACTION_START_CMD(int32 monster_serial, float start_position)
{
	AAZMonster* target = GetMonster(monster_serial);
	if (!target) return;
	
	FCG_MONSTER_ACTION_START_CMD packet(target->object_serial_);
	packet.state_info = MonsterActionStateInfo(target->action_state_info_);

	//TODO SEND
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Server][#%d][Send_FCG_MONSTER_ACTION_START_CMD] Action name: %s, Montage section: %s, Start from: %f"),
		target->object_serial_, *target->action_state_info_.animation_name.ToString(),
		*target->action_state_info_.montage_section_name.ToString(), start_position);
}

void UAZMonsterPacketMgr_Server::Send_FCG_MONSTER_PART_CHANGE_CMD(int32 monster_serial, EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type)
{
	AAZMonster* target = GetMonster(monster_serial);
	if (!target) return;
	
	FCG_MONSTER_PART_CHANGE_CMD packet(target->object_serial_);
	packet.body_part = body_part;
	packet.change_type = change_type;

	//TODO SEND
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Server][#%d][Receive_FCG_MONSTER_PART_CHANGE_CMD], %s %s"),
		target->object_serial_, *UAZUtility::EnumToString(body_part), *UAZUtility::EnumToString(change_type));
}

void UAZMonsterPacketMgr_Server::Send_FCG_PLAYER_ATTACK_HIT_CMD(int32 monster_serial, int32 attacker_serial, FVector hit_pos, int32 damage_amount)
{
	AAZMonster* target = GetMonster(monster_serial);
	if (!target) return;
	
	FCG_PLAYER_ATTACK_HIT_CMD packet(target->object_serial_);
	packet.player_serial = attacker_serial;
	packet.hit_position = hit_pos;
	packet.damage_amount = damage_amount;

	//TODO SEND
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketMgr_Server][#%d][Receive_FCG_PLAYER_ATTACK_HIT_CMD], From: %d, Damage dealt: %d, Position: %s"),
		target->object_serial_, attacker_serial, damage_amount, *hit_pos.ToString());
}
