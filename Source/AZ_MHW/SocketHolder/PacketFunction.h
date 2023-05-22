#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "PacketFunction.generated.h"


class AAZMonster_Client;

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
#pragma region Client->Server packet handlers
	// Login
	void LoginRequest(UINT32 client_index, LOGIN_REQUEST_PACKET* packet);
	void SignupRequest(UINT32 client_index, LOGIN_REQUEST_PACKET* packet);

	// Chat
	void RequestChatting(UINT32 client_index, LOGIN_REQUEST_PACKET* packet);
	
	// Map
	void RequestInGame(/*todo*/);

	// Character
	void RequestPlayerMove(/*todo*/);
#pragma endregion

#pragma region Server->Client packet handlers
	// Login
	void LoginResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed);
	void SigninResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed);

	// Monster
	AAZMonster_Client* GetMonster_Client(int32 object_serial);
	void Receive_SC_MONSTER_SPAWN_CMD(const SC_MONSTER_SPAWN_CMD* packet);
	void Receive_SC_MONSTER_TRANSFORM_CMD(const SC_MONSTER_TRANSFORM_CMD* packet);
	void Receive_SC_MONSTER_BODY_STATE_CMD(const SC_MONSTER_BODY_STATE_CMD* packet);
	void Receive_SC_MONSTER_ENTER_COMBAT_CMD(const SC_MONSTER_ENTER_COMBAT_CMD* packet);
	void Receive_SC_MONSTER_ACTION_START_CMD(const SC_MONSTER_ACTION_START_CMD* packet);
	void Receive_SC_MONSTER_PART_CHANGE_CMD(const SC_MONSTER_PART_CHANGE_CMD* packet);
	void Receive_SC_MONSTER_HIT_CMD(const SC_MONSTER_HIT_CMD* packet); 
	void Receive_SC_MONSTER_DIE_CMD(const SC_MONSTER_DIE_CMD* packet); 
#pragma endregion

private:
	UPROPERTY() TWeakObjectPtr<class UAZGameInstance> game_instance_;
};
