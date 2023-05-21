#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/SocketHolder/PacketFunction.h"

bool UAZSocketHolder::ProcessPacket(PACKET_HEADER* recv_packet)
{
	FString out_request_protocol("");
	switch ((PACKET_ID)recv_packet->packet_id)
	{
	case PACKET_ID::LOGIN_RESPONSE_SUCCESS:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::LoginResponse(packet, true);
		OutRequestProtocol(PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::LOGIN_RESPONSE_FAIL:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::LoginResponse(packet, false);
		OutRequestProtocol(PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::SIGNIN_RESPONSE_SUCCESS:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::SigninResponse(packet, true);
		OutRequestProtocol(PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::SIGNIN_RESPONSE_FAIL:
	{
		LOGIN_RESPONSE_PACKET* packet = (LOGIN_RESPONSE_PACKET*)recv_packet;
		UPacketFunction::SigninResponse(packet, false);
		OutRequestProtocol(PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS:
	{
		OutRequestProtocol(PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::CHAT_SEND_RESPONSE_FAIL:
	{
		OutRequestProtocol(PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case PACKET_ID::IN_GAME_SUCCESS:
	{
		OutRequestProtocol(PACKET_ID::IN_GAME_REQUEST, out_request_protocol);
	}
	break;
	default:
	{
		return false;
	}
	break;
	}

	ScreenWaitProc(out_request_protocol);
	return true;
}

