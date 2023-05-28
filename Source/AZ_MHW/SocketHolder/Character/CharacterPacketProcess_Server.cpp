#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "GameInstance/AZGameInstance.h"
#include "PlayerController/AZPlayerController_Server.h"

void UPacketFunction::CreatePlayerOriginRequest(UINT32 client_index)
{
	auto server_controller =Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->AddPlayer_Origin(client_index);
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

void UPacketFunction::DevelopPlayerForceUpdateCommand(UINT32 client_index, ACTION_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->TempDevelopForceUpdatePlayer_Origin(client_index, packet->current_position);
}

