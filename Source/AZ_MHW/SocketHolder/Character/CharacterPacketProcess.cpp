#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "GameInstance/AZGameInstance.h"
#include "PlayerController/AZPlayerController_Server.h"

void UPacketFunction::RequestOriginPlayerCreate(UINT32 client_index)
{
	auto player_controller =Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());
	player_controller->Origin_AddPlayer(client_index);

}

void UPacketFunction::RequestOriginPlayerDestroy(UINT32 client_index)
{
	auto player_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());
	player_controller->Origin_RemovePlayer(client_index);

}

void UPacketFunction::RequestOriginPlayerAction(UINT32 client_index, ACTION_PLAYER_PACKET* packet)
{
	auto player_controller = Cast<AAZPlayerController_Server>(game_instance_->GetPlayerController());

	packet->current_direction;
	packet->current_position;
	packet->input_direction;
	packet->input_data;
	
	//player_controller->Origin_ControlPlayer(client_index, packet);
}

void UPacketFunction::RequestOriginPlayerEquipment(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet)
{
	
}

void UPacketFunction::CommandUpdatePlayerState(UINT32 client_index, UPDATE_PLAYER_STATE_PACKET* packet)
{
	
}

void UPacketFunction::ReqeustPlayerCharacterCreate()
{
	//DB 
	//
	//
	//
}

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


