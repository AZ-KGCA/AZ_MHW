#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameMode/AZGameMode_InGame.h"
#include "AZ_MHW/Manager/AZObjectMgr_Client.h"
#include "CharacterComponent/AZMonsterPacketHandlerComponent_Client.h"


AAZMonster_Client* UPacketFunction::GetMonster_Client(int32 object_serial)
{
	const auto gamemode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode());
	if (!gamemode)
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][Receive_SC_MONSTER_SPAWN_CMD] Incorrect gamemode!"));
		return nullptr;
	}
	return gamemode->object_mgr_->GetMonster(object_serial);
}

void UPacketFunction::Receive_SC_MONSTER_SPAWN_CMD(const SC_MONSTER_SPAWN_CMD* packet)
{
	const auto gamemode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode());
	if (!gamemode)
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][Receive_SC_MONSTER_SPAWN_CMD] Incorrect gamemode!"));
		return;
	}
	gamemode->object_mgr_->SpawnMonster(packet->object_serial, packet->monster_id, packet->location, packet->rotation);
	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_SPAWN_CMD] ID: %d, Location: %s, Rotation %s"),
		packet->object_serial, packet->monster_id, *packet->location.ToString(), *packet->rotation.ToString());
}

void UPacketFunction::Receive_SC_MONSTER_TRANSFORM_CMD(const SC_MONSTER_TRANSFORM_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;

	target_monster->packet_handler_component_->Receive_SC_MONSTER_TRANSFORM_CMD(packet->location, packet->rotation, packet->is_forced);
}

void UPacketFunction::Receive_SC_MONSTER_BODY_STATE_CMD(const SC_MONSTER_BODY_STATE_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;

	target_monster->packet_handler_component_->Receive_SC_MONSTER_BODY_STATE_CMD(
		packet->head_state, packet->back_state, packet->left_wing_state, packet->right_wing_state, packet->tail_state);
}

void UPacketFunction::Receive_SC_MONSTER_ENTER_COMBAT_CMD(const SC_MONSTER_ENTER_COMBAT_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_ENTER_COMBAT_CMD();
}

void UPacketFunction::Receive_SC_MONSTER_ACTION_START_CMD(const SC_MONSTER_ACTION_START_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;

	MonsterActionStateInfo state_info_temp = packet->state_info;
	FMonsterActionStateInfo state_info = state_info_temp.Convert();
	target_monster->packet_handler_component_->Receive_SC_MONSTER_ACTION_START_CMD(state_info, packet->start_position);
}

void UPacketFunction::Receive_SC_MONSTER_PART_CHANGE_CMD(const SC_MONSTER_PART_CHANGE_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_PART_CHANGE_CMD(packet->body_part, packet->change_type);
}

void UPacketFunction::Receive_SC_MONSTER_HIT_CMD(const SC_MONSTER_HIT_CMD* packet)
{
	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (!target_monster) return;
	
	target_monster->packet_handler_component_->Receive_SC_MONSTER_HIT_CMD(packet->hit_position, packet->damage_amount);
}

void UPacketFunction::Receive_SC_MONSTER_DIE_CMD(const SC_MONSTER_DIE_CMD* packet)
{
	const auto gamemode = Cast<AAZGameMode_InGame>(game_instance_->GetGameMode());
	if (!gamemode)
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][Receive_SC_MONSTER_SPAWN_CMD] Incorrect gamemode!"));
		return;
	}
	
	if (gamemode->object_mgr_->RemoveObject(packet->object_serial))
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_DIE_CMD] %d removal succeeded"), packet->object_serial);
	}
	else
	{
		UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_DIE_CMD] %d removal failed"), packet->object_serial);
	}

	AAZMonster_Client* target_monster = GetMonster_Client(packet->object_serial);
	if (target_monster)
		target_monster->packet_handler_component_->Receive_SC_MONSTER_DIE_CMD();
}

