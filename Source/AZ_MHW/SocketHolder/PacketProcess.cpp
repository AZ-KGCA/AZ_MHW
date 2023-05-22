#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"

void UPacketFunction::Init()
{
	game_instance_ = Cast<UAZGameInstance>(GetOuter());
}

bool UPacketFunction::ProcessPacket(UINT32 client_index, PACKET_HEADER* recv_packet)
{
	// Process packets from client to server
	switch (static_cast<PACKET_ID>(recv_packet->packet_id))
	{
	case PACKET_ID::LOGIN_REQUEST:
	{
		LOGIN_REQUEST_PACKET* packet = (LOGIN_REQUEST_PACKET*)recv_packet;
		UPacketFunction::LoginRequest(client_index, packet);
	}
	break;
	case PACKET_ID::SIGNIN_REQUEST:
	{
		LOGIN_REQUEST_PACKET* packet = (LOGIN_REQUEST_PACKET*)recv_packet;
		UPacketFunction::SignupRequest(client_index, packet);
	}
	break;
	case PACKET_ID::CHAT_SEND_REQUEST:
	{
		LOGIN_REQUEST_PACKET* packet = (LOGIN_REQUEST_PACKET*)recv_packet;
		UPacketFunction::RequestChatting(client_index, packet);
	}
	break;
	case PACKET_ID::IN_GAME_REQUEST:
	{
		UPacketFunction::RequestInGame(/*todo*/);
	}
	break;
	case PACKET_ID::IN_GAME_INPUT_REQUEST:
	{
		UPacketFunction::RequestPlayerMove(/*todo*/);
	}
	break;
	default:
		break;
	}
//===========================================================================================================================================//
	// Process packets from server to client
	FString out_request_protocol("");
	switch (static_cast<PACKET_ID>(recv_packet->packet_id))
	{
	case PACKET_ID::LOGIN_RESPONSE_SUCCESS:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::LoginResponse(packet, true);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::LOGIN_RESPONSE_FAIL:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::LoginResponse(packet, false);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::SIGNIN_RESPONSE_SUCCESS:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::SigninResponse(packet, true);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::SIGNIN_RESPONSE_FAIL:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::SigninResponse(packet, false);
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS:
	{
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::CHAT_SEND_RESPONSE_FAIL:
	{
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::IN_GAME_SUCCESS:
	{
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::IN_GAME_REQUEST, out_request_protocol);
	}
	break;

	// Monster related packets
	case PACKET_ID::SC_MONSTER_SPAWN_CMD:
	{
		const SC_MONSTER_SPAWN_CMD* packet = reinterpret_cast<SC_MONSTER_SPAWN_CMD*>(recv_packet);
		UPacketFunction::Receive_SC_MONSTER_SPAWN_CMD(packet);
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
		return false;
	}
	break;
	}

	game_instance_->GetSocketHolder(ESocketHolderType::Game)->ScreenWaitProc(out_request_protocol);
	return true;
}
