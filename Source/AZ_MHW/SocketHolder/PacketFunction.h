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

public:
	// server(character)
	void RequestPlayerMove(/*todo*/);

private:
	UPROPERTY() class UAZGameInstance* game_instance_;
};
