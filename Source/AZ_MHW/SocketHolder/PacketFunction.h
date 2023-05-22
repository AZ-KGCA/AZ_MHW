#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "PacketFunction.generated.h"


UCLASS()
class AZ_MHW_API UPacketFunction : public UObject
{
	// static 함수로 만들시 gameinstance의 종류를 찾지를 못함(인스턴스 2개 실행시)
	GENERATED_BODY()

public:
	UPacketFunction() {}

public:
	bool ProcessPacket(UINT32 client_index, PACKET_HEADER* recv_packet);
	void Init();

public:
	// client(Login)
	void LoginResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed);
	void SigninResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed);

public:
	// server(Login)
	void LoginRequest(UINT32 client_index, LOGIN_REQUEST_PACKET* packet);
	void SignupRequest(UINT32 client_index, LOGIN_REQUEST_PACKET* packet);

public:
	// server(Chat)
	void RequestChatting(UINT32 client_index, LOGIN_REQUEST_PACKET* packet);

public:
	// map
	void RequestInGame(/*todo*/);

#pragma region PlayerCharacterParts
public:
#pragma region Client->Server
	void RequestOriginPlayerCreate(UINT32 client_index);
	void RequestOriginPlayerDestroy(UINT32 client_index);
	
	void RequestOriginPlayerAction(UINT32 client_index, ACTION_PLAYER_PACKET* packet);
	void RequestOriginPlayerEquipment(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet);
	void CommandUpdatePlayerState(UINT32 client_index, UPDATE_PLAYER_STATE_PACKET* packet);

	void ReqeustPlayerCharacterCreate();
	void RequestPlayerCharacterDestroy();//플레이어 캐릭터 제거
#pragma endregion
#pragma region Server->Client
	//void CommandUpdateField(UINT32 client_index);
	//void CommandUpdateMerchant(UINT32 client_index);
	
	void CommandCreateRemotablePlayer(UINT32 client_index);
	void CommandDestroyRemotablePlayer(UINT32 client_index);
	void CommandActionRemotablePlayer(UINT32 client_index, ACTION_PLAYER_PACKET* packet);
	void CommandEquipRemotablePlayer(UINT32 client_index, EQUIPMENT_PLAYER_PACKET* packet);
	void CommandUpdateRemotablePlayerState(UINT32 client_index, UPDATE_PLAYER_STATE_PACKET* packet);
	
	void ResponePlayerCharacterCreate();//플레이어 생성후 데이터받기
	void ResponePlayerCharacterDestroy();//플레이어 제거후 데이터받기
	void RequestDisplayPlayerDemage(UINT32 client_index);//플레이어가 준 데미지 표시
	
#pragma endregion 
#pragma endregion 
private:
	UPROPERTY() class UAZGameInstance* game_instance_;
};
