#include "AZ_MHW/SocketHolder/PacketFunction.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/SocketHolder/PacketFunction.h"
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
	
	//client->server
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
#pragma region Character_PART
	case PACKET_ID::CS_PLAYER_ORIGIN_CREATE_REQ:
		//접속햇으니 서버에 생성하세요.
		UPacketFunction::RequestOriginPlayerCreate(client_index);
		break;
	case PACKET_ID::CS_PLAYER_ORIGIN_DESTROY_REQ:
		//접속종료햇으니 서버에서 제거하세요.
		UPacketFunction::RequestOriginPlayerDestroy(client_index);
		break;
	case PACKET_ID::CS_PLAYER_ORIGIN_ACTION_REQ:
		//입력햇으니 액션하세요.
		
		break;
		
	case PACKET_ID::CS_PLAYER_ORIGIN_EQUIP_REQ:
		//장비변경햇으니 변경하시고.

		break;
	case PACKET_ID::CS_PLAYER_LOCAL_GUID_REQ:
		//GUID 주세요.

		break;
	case PACKET_ID::CS_PLAYER_LOCAL_CHARACTER_DATA_REQ:
		//데이터 주세요.

		break;
	case PACKET_ID::CS_PLAYER_LOCAL_CHARACTER_CREATE_REQ:
		//캐릭터 생성해주세요.

		break;
	case PACKET_ID::CS_PLAYER_LOCAL_CHARACTER_DESTROY_REQ:
		//캐릭터 제거해주세요.

		break;
#pragma endregion 
	default:
	{
		is_server_packet = false;
	}
	break;
	}
//===========================================================================================================================================//
	//server->client
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
#pragma region Character_PART
	case PACKET_ID::SC_PLAYER_REMOTABLE_CREATE_CMD:
		//원격 생성명령

		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_DESTROY_CMD:
		//원격 제거명령

		break;
	case PACKET_ID::SC_ENVIRONMENT_UPDATE_FIELD_CMD:
		//필드 갱신명령

		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_EQUIP_CMD:
		//원격 장비명령

		break;
	case PACKET_ID::SC_PLAYER_REMOTABLE_ACTION_CMD:
		//원격 액션명령

		break;

	case PACKET_ID::SC_PLAYER_LOCAL_CHARACTER_DATA_RES:
		//캐릭터 데이터 받기

		break;
	case PACKET_ID::SC_PLAYER_LOCAL_CHARACTER_CREATE_RES:
		//캐릭터 생성 받기

		break;
	case PACKET_ID::SC_PLAYER_LOCAL_CHARACTER_DESTROY_RES:
		//캐릭터 파괴 받기

		break;
#pragma endregion 
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
