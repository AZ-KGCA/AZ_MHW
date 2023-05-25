#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "GameInstance/AZGameInstance.h"
#include "PlayerController/AZPlayerController_Server.h"


#pragma region Server
void UPacketFunction::PlayerOriginCreateRequest(UINT32 client_index)
{
	auto server_controller =Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());
	//원격 접속생성후
	server_controller->AddPlayer_Origin(client_index);
	//브로드캐스트 리모터블 플레이어생성
	//server_controller->
	
}

void UPacketFunction::PlayerOriginDestroyRequest(UINT32 client_index)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->RemovePlayer_Origin(client_index);
}

void UPacketFunction::PlayerOriginActionRequest(UINT32 client_index, ACTION_PLAYER_PACKET* packet)
{
	auto server_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	server_controller->ActionPlayer_Origin(
		client_index,
		packet->current_position,
		packet->current_direction,
		packet->input_direction,
		packet->input_data);
}

void UPacketFunction::PlayerOriginEquipmentRequest(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet)
{
	
}

void UPacketFunction::PlayerStateUpdateCommand(UINT32 client_index, UPDATE_PLAYER_STATE_PACKET* packet)
{
	
}

void UPacketFunction::RequestPlayerCharacterCreate()
{
	//DB 
	
	//
}
#pragma endregion

#pragma region Client
void UPacketFunction::CommandCreateRemotablePlayer(UINT32 client_index)
{
	
}

void UPacketFunction::CommandDestroyRemotablePlayer(UINT32 client_index)
{
	
}

void UPacketFunction::CommandActionRemotablePlayer(UINT32 client_index, ACTION_PLAYER_PACKET* packet)
{
	
}

void UPacketFunction::CommandEquipRemotablePlayer(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet)
{
	
}

void UPacketFunction::CommandUpdateRemotablePlayerState(UINT32 client_index, UPDATE_PLAYER_STATE_PACKET* packet)
{
	
}
#pragma endregion

