#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/CommonSource/AZLog.h"

void UPacketFunction::Init()
{
	game_instance_ = Cast<UAZGameInstance>(GetOuter());
}

bool UPacketFunction::ProcessPacket(UINT32 client_index, PACKET_HEADER* recv_packet)
{
	bool is_server_packet = true;
	bool is_client_packet = true;
	
	// Process packets from client to server
	switch ((PACKET_ID)recv_packet->packet_id)
	{
	case PACKET_ID::CS_LOGIN_SIGNIN_REQ:
	{
		CS_LOGIN_SIGNIN_REQ* packet = (CS_LOGIN_SIGNIN_REQ*)recv_packet;
		UPacketFunction::LoginSigninRequest(client_index, packet);
	}
	break;
	case PACKET_ID::CS_LOGIN_SIGNUP_REQ:
	{
		CS_LOGIN_SIGNUP_REQ* packet = (CS_LOGIN_SIGNUP_REQ*)recv_packet;
		UPacketFunction::LoginSignupRequest(client_index, packet);
	}
	break;
	case PACKET_ID::CS_CHAT_MSG_CMD:
	{
		CS_CHAT_MSG_CMD* packet = (CS_CHAT_MSG_CMD*)recv_packet;
		UPacketFunction::ChatMsgCommand(client_index, packet);
	}
	break;
	case PACKET_ID::CS_ITEM_TOTAL_INFO_REQ:
	{
		CS_CHAT_MSG_CMD* packet = (CS_CHAT_MSG_CMD*)recv_packet;
		UPacketFunction::ChatMsgCommand(client_index, packet);
	}
	break;
	case PACKET_ID::CS_ITEM_UNEQUIP_REQ:
	{
		CS_CHAT_MSG_CMD* packet = (CS_CHAT_MSG_CMD*)recv_packet;
		UPacketFunction::ChatMsgCommand(client_index, packet);
	}
	break;
	case PACKET_ID::CS_ITEM_EQUIP_REQ:
	{
		CS_ITEM_EQUIP_REQ* packet = (CS_ITEM_EQUIP_REQ*)recv_packet;
		UPacketFunction::ItemEquipRequest(client_index, packet);
	}
	break;
	case PACKET_ID::CS_ITEM_MOVE_REQ:
	{
		CS_ITEM_MOVE_REQ* packet = (CS_ITEM_MOVE_REQ*)recv_packet;
		UPacketFunction::ItemMoveRequest(client_index, packet);
	}
	break;
	case PACKET_ID::CS_ITEM_CREATE_REQ:
	{
		CS_ITEM_CREATE_REQ* packet = (CS_ITEM_CREATE_REQ*)recv_packet;
		UPacketFunction::ItemCreateRequest(client_index, packet);
	}
	break;
	case PACKET_ID::CS_ITEM_USE_REQ:
	{
		CS_ITEM_USE_REQ* packet = (CS_ITEM_USE_REQ*)recv_packet;
		UPacketFunction::ItemUseRequest(client_index, packet);
	}
	break;
	case PACKET_ID::CS_COMBAT_MAP_ENTER_REQ:
	{
		CS_COMBAT_MAP_ENTER_REQ* packet = (CS_COMBAT_MAP_ENTER_REQ*)recv_packet;
		UPacketFunction::Receive_CS_COMBAT_MAP_ENTER_REQ(client_index, packet);
	}
	break;
	case PACKET_ID::CS_COMBAT_MAP_LOAD_FINISH_CMD:
	{
		CS_COMBAT_MAP_LOAD_FINISH_CMD* packet = (CS_COMBAT_MAP_LOAD_FINISH_CMD*)recv_packet;
		UPacketFunction::Receive_CS_COMBAT_MAP_LOAD_FINISH_CMD(client_index, packet);
	}
	break;
	case PACKET_ID::CS_MONSTER_UPDATE_REQ:
	{
		CS_MONSTER_UPDATE_REQ* packet = (CS_MONSTER_UPDATE_REQ*)recv_packet;
		UPacketFunction::Receive_CS_MONSTER_UPDATE_REQ(client_index, packet);
	}
	break;
	default:
	{
		is_server_packet = false;
	}
	break;
	}
//===========================================================================================================================================//
	// Process packets from server to client
	FString out_request_protocol("");
	switch ((PACKET_ID)recv_packet->packet_id)
	{
	case PACKET_ID::CS_LOGIN_SIGNIN_RES:
	{
		CS_LOGIN_SIGNIN_RES* packet = (CS_LOGIN_SIGNIN_RES*)recv_packet;
		UPacketFunction::LoginSigninResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_LOGIN_SIGNIN_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::CS_LOGIN_SIGNUP_RES:
	{
		CS_LOGIN_SIGNUP_RES* packet = (CS_LOGIN_SIGNUP_RES*)recv_packet;
		UPacketFunction::LoginSignupResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_LOGIN_SIGNUP_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::SC_CHAT_MSG_CMD:
	{
		SC_CHAT_MSG_CMD* packet = (SC_CHAT_MSG_CMD*)recv_packet;
		UPacketFunction::ChatMsgCommand(packet);
	}
	break;
	case PACKET_ID::CS_ITEM_TOTAL_INFO_RES:
	{
		CS_ITEM_TOTAL_INFO_RES* packet = (CS_ITEM_TOTAL_INFO_RES*)recv_packet;
		UPacketFunction::ItemTotalInfoResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_ITEM_TOTAL_INFO_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::CS_ITEM_UNEQUIP_RES:
	{
		CS_ITEM_UNEQUIP_RES* packet = (CS_ITEM_UNEQUIP_RES*)recv_packet;
		UPacketFunction::ItemUnEquipResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_ITEM_UNEQUIP_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::CS_ITEM_EQUIP_RES:
	{
		CS_ITEM_EQUIP_RES* packet = (CS_ITEM_EQUIP_RES*)recv_packet;
		UPacketFunction::ItemEquipResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_ITEM_EQUIP_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::CS_ITEM_MOVE_RES:
	{
		CS_ITEM_MOVE_RES* packet = (CS_ITEM_MOVE_RES*)recv_packet;
		UPacketFunction::ItemMoveResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_ITEM_MOVE_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::CS_ITEM_CREATE_RES:
	{
		CS_ITEM_CREATE_RES* packet = (CS_ITEM_CREATE_RES*)recv_packet;
		UPacketFunction::ItemCreateResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_ITEM_CREATE_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::CS_ITEM_USE_RES:
	{
		CS_ITEM_USE_RES* packet = (CS_ITEM_USE_RES*)recv_packet;
		UPacketFunction::ItemUseResponse(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_ITEM_USE_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::SC_ITEM_INFO_CMD:
	{
		SC_ITEM_INFO_CMD* packet = (SC_ITEM_INFO_CMD*)recv_packet;
		UPacketFunction::ItemInfoCommand(packet);
	}
	break;

	// Monster related packets
	case PACKET_ID::SC_MONSTER_SPAWN_CMD:
	{
		const SC_MONSTER_SPAWN_CMD* packet = reinterpret_cast<SC_MONSTER_SPAWN_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_SPAWN_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_SPAWN_END_CMD:
	{
		const SC_MONSTER_SPAWN_END_CMD* packet = reinterpret_cast<SC_MONSTER_SPAWN_END_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_SPAWN_END_CMD(packet);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CS_COMBAT_MAP_ENTER_REQ, out_request_protocol);
	}
	break;
	case PACKET_ID::SC_MONSTER_TRANSFORM_CMD:
	{
		const SC_MONSTER_TRANSFORM_CMD* packet = reinterpret_cast<SC_MONSTER_TRANSFORM_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_TRANSFORM_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_BODY_STATE_CMD:
	{
		const SC_MONSTER_BODY_STATE_CMD* packet = reinterpret_cast<SC_MONSTER_BODY_STATE_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_BODY_STATE_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_ENTER_COMBAT_CMD:
	{
		const SC_MONSTER_ENTER_COMBAT_CMD* packet = reinterpret_cast<SC_MONSTER_ENTER_COMBAT_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_ENTER_COMBAT_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_ACTION_START_CMD:
	{
		const SC_MONSTER_ACTION_START_CMD* packet = reinterpret_cast<SC_MONSTER_ACTION_START_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_ACTION_START_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_ACTION_END_CMD:
	{
		const SC_MONSTER_ACTION_END_CMD* packet = reinterpret_cast<SC_MONSTER_ACTION_END_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_ACTION_END_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_PART_CHANGE_CMD:
	{
		const SC_MONSTER_PART_CHANGE_CMD* packet = reinterpret_cast<SC_MONSTER_PART_CHANGE_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_PART_CHANGE_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_HIT_CMD:
	{
		const SC_MONSTER_HIT_CMD* packet = reinterpret_cast<SC_MONSTER_HIT_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_HIT_CMD(packet);
	}
	break;
	case PACKET_ID::SC_MONSTER_DIE_CMD:
	{
		const SC_MONSTER_DIE_CMD* packet = reinterpret_cast<SC_MONSTER_DIE_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_DIE_CMD(packet);
	}
	break;
			
	default:
	{
		is_client_packet = false;
	}
	break;
	}

	if ((is_server_packet || is_client_packet) == false)
	{
		AZ_LOG("Packet Not Process : id : %d", recv_packet->packet_id);
	}

	game_instance_->GetSocketHolder(ESocketHolderType::Game)->ScreenWaitProc(out_request_protocol);
	return (is_server_packet || is_client_packet);
}
