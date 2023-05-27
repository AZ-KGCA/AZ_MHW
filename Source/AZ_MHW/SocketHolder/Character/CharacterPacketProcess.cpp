#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "GameInstance/AZGameInstance.h"
#include "PlayerController/AZPlayerController_InGame.h"
#include "PlayerController/AZPlayerController_Server.h"
#include "PlayerState/AZPlayerState_Client.h"

#pragma region Server
void UPacketFunction::CreatePlayerOriginRequest(UINT32 client_index)
{
	auto server_controller =Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->AddPlayer_Origin(client_index);
	//TODO 접속중인 모드 유저에게 브로드캐스트 리모터블 플레이어 생성
	//TODO 본인에게 모든 접속중인 리모터블 플레이어 생성
}

void UPacketFunction::DestroyPlayerOriginRequest(UINT32 client_index)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->RemovePlayer_Origin(client_index);
	//TODO 접속중인 모든 유저에게 브로드캐스트 리모터블 플레이어제거
}

void UPacketFunction::ActionPlayerOriginRequest(UINT32 client_index, ACTION_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->ActionPlayer_Origin(
		client_index,
		packet->current_position,
		packet->current_direction,
		packet->input_direction,
		packet->input_data);
	//TODO 접송중인 모든 유저에게 브로드캐스트 리모터블 액션
}

void UPacketFunction::EquipPlayerOriginRequest(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->EquipPlayer_Origin(client_index, packet->item_id);
	//TODO 접속중인 모든 유저에게 브로드캐스트 장비변경
}

void UPacketFunction::UpdatePlayerStateCommand(UPDATE_PLAYER_STATE_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	//server_controller->EquipPlayer_Origin(client_index, packet->item_id);
	//TODO 접속중인 유저에게
}


#pragma endregion

#pragma region Client
void UPacketFunction::ProcessCreatePlayer_Playable()
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->AddPlayer_Playable();
}

void UPacketFunction::ProcessCreatePlayer_Remotable(INITIALIZE_PLAYER_STATE_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());
	
	FAZPlayerCharacterState character_state;
	character_state.character_position = packet->pos;
	character_state.character_direction.Yaw = packet->dir;
	
	FAZPlayerEquipmentState equipment_state;
	equipment_state.head_item_id = packet->head_id;
	equipment_state.hair_item_id = packet->hair_id;
	equipment_state.arm_item_id = packet->arm_id;
	equipment_state.body_item_id = packet->body_id;
	equipment_state.waist_item_id = packet->waist_id;
	equipment_state.leg_item_id = packet->leg_id;
	
	equipment_state.weapon_type = packet->weapon_id;
	equipment_state.first_weapon_item_id = packet->first_id;
	equipment_state.second_weapon_item_id = packet->second_id;

	player_controller->AddPlayerState_Remotable(packet->guid, character_state, equipment_state);
}

void UPacketFunction::CreatePlayerRemotableCommand(CREATE_PLAYER_CHARACTER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());
	
	player_controller->AddPlayer_Remotable(packet->guid);
}

void UPacketFunction::DestroyPlayerRemotableCommand(DESTROY_PLAYER_CHARACTER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->RemovePlayer_Remotable(packet->guid);
}

void UPacketFunction::ActionPlayerRemotableCommand(ACTION_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());

	player_controller->ActionPlayer_Remotable(
	packet->guid,
	packet->current_position,
	packet->current_direction,
	packet->input_direction,
	packet->input_data);
}

void UPacketFunction::EquipPlayerRemotableCommand( EQUIPMENT_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());
	
}

void UPacketFunction::UpdatePlayerStateRemotableCommand( UPDATE_PLAYER_STATE_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_InGame>(game_instance_->GetPlayerController());
	
}
#pragma endregion

