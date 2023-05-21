#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/GameInstance/Client_Packet.h"
#include "AZ_MHW/SocketHolder/PacketFunction.h"

bool UAZSocketHolder::ProcessPacket(BasePacket* recv_packet)
{
	FString out_request_protocol("");
	switch ((CLIENT_PACKET_ID)recv_packet->packet_id)
	{
	case CLIENT_PACKET_ID::LOGIN_RESPONSE_SUCCESS:
	{
		Login_Send_Packet* packet = (Login_Send_Packet*)recv_packet;
		UPacketFunction::LoginResponse(packet, true);
		OutRequestProtocol(CLIENT_PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case CLIENT_PACKET_ID::LOGIN_RESPONSE_FAIL:
	{
		Login_Send_Packet* packet = (Login_Send_Packet*)recv_packet;
		UPacketFunction::LoginResponse(packet, false);
		OutRequestProtocol(CLIENT_PACKET_ID::LOGIN_REQUEST, out_request_protocol);
	}
	break;
	case CLIENT_PACKET_ID::SIGNIN_RESPONSE_SUCCESS:
	{
		Login_Send_Packet* packet = (Login_Send_Packet*)recv_packet;
		UPacketFunction::SigninResponse(packet, true);
		OutRequestProtocol(CLIENT_PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case CLIENT_PACKET_ID::SIGNIN_RESPONSE_FAIL:
	{
		Login_Send_Packet* packet = (Login_Send_Packet*)recv_packet;
		UPacketFunction::SigninResponse(packet, false);
		OutRequestProtocol(CLIENT_PACKET_ID::SIGNIN_REQUEST, out_request_protocol);
	}
	break;
	case CLIENT_PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS:
	{
		OutRequestProtocol(CLIENT_PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case CLIENT_PACKET_ID::CHAT_SEND_RESPONSE_FAIL:
	{
		OutRequestProtocol(CLIENT_PACKET_ID::CHAT_SEND_REQUEST, out_request_protocol);
	}
	break;
	case CLIENT_PACKET_ID::IN_GAME_SUCCESS:
	{
		OutRequestProtocol(CLIENT_PACKET_ID::IN_GAME_REQUEST, out_request_protocol);
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

