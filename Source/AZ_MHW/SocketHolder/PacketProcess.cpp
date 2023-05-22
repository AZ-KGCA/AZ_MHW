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
	bool result = true;
	//client->server
	switch ((PACKET_ID)recv_packet->packet_id)
	{
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
		const auto packet = (ACTION_PLAYER_PACKET*)recv_packet;
		UPacketFunction::RequestOriginPlayerAction(client_index, packet);
		break;
	case PACKET_ID::CS_PLAYER_ORIGIN_EQUIP_REQ:
		//장비변경햇으니 변경하시고.
		
		break;
	case PACKET_ID::CS_PLAYER_GUID_REQ:
		//GUID 주세요.
		
		break;
	case PACKET_ID::CS_PLAYER_CHARACTER_DATA_REQ:
		//데이터 주세요.
		
		break;
	case PACKET_ID::CS_PLAYER_CHARACTER_CREATE_REQ:
		//캐릭터 생성해주세요.
		
		break;
	case PACKET_ID::CS_PLAYER_CHARACTER_DESTROY_REQ:
		//캐릭터 제거해주세요.
		
		break;
#pragma endregion 
	default:
		
		break;
	}
//===========================================================================================================================================//
	//server->client
	FString out_request_protocol("");
	switch ((PACKET_ID)recv_packet->packet_id)
	{
#pragma region Character_PART
	case PACKET_ID::SC_BROADCAST_CREATE_PLAYER_CMD:
		//원격 생성명령
		
		break;
	case PACKET_ID::SC_BROADCAST_DESTROY_PLAYER_CMD:
		//원격 제거명령
		
		break;
	case PACKET_ID::SC_BROADCAST_UPDATE_FIELD_CMD:
		//필드 갱신명령
		
		break;
	case PACKET_ID::SC_BROADCAST_EQUIP_PLAYER_CMD:
		//원격 장비명령
		
		break;
	case PACKET_ID::SC_BROADCAST_ACTION_PLAYER_CMD:
		//원격 액션명령
		
		break;
		
	case PACKET_ID::SC_PLAYER_CHARACTER_DATA_RES:
		//캐릭터 데이터 받기
		
		break;
	case PACKET_ID::SC_PLAYER_CHARACTER_CREATE_RES:
		//캐릭터 생성 받기
		
		break;
	case PACKET_ID::SC_PLAYER_CHARACTER_DESTROY_RES:
		//캐릭터 파괴 받기
		
		break;
		
#pragma endregion 
	default:
	{
		result = false;
	}
	break;
	}

	game_instance_->GetSocketHolder(ESocketHolderType::Game)->ScreenWaitProc(out_request_protocol);
	return result;
}
