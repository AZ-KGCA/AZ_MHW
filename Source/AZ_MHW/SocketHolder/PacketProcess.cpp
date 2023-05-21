#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"

void UPacketFunction::Init()
{
	game_instance_ = Cast<UAZGameInstance>(GetOuter());
}

bool UPacketFunction::ProcessPacket(UINT32 client_index, PACKET_HEADER* recv_packet)
{
	//server
	switch ((PACKET_ID)recv_packet->packet_id)
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
	}
//===========================================================================================================================================//
	//client
	FString out_request_protocol("");
	switch ((PACKET_ID)recv_packet->packet_id)
	{
	case PACKET_ID::LOGIN_RESPONSE_SUCCESS:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::LoginResponse(packet, true);
		AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::LOGIN_RESPONSE_FAIL:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::LoginResponse(packet, false);
		AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::SIGNIN_RESPONSE_SUCCESS:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::SigninResponse(packet, true);
		AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::SIGNIN_RESPONSE_FAIL:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::SigninResponse(packet, false);
		AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS:
	{
		AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::CHAT_SEND_RESPONSE_FAIL:
	{
		AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::IN_GAME_SUCCESS:
	{
		AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->OutRequestProtocol(PACKET_ID::IN_GAME_REQUEST, out_request_protocol);
	}
	break;
	default:
	{
		return false;
	}
	break;
	}

	AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->ScreenWaitProc(out_request_protocol);
	return true;
}
