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
	// server(Login)
	void LoginSigninRequest(UINT32 client_index, CS_LOGIN_SIGNIN_REQ* packet);
	void LoginSignupRequest(UINT32 client_index, CS_LOGIN_SIGNUP_REQ* packet);
	// client(Login)
	void LoginSigninResponse(CS_LOGIN_SIGNIN_RES* packet);
	void LoginSignupResponse(CS_LOGIN_SIGNUP_RES* packet);

public:
	// server(chat)
	void ChatMsgCommand(UINT32 client_index, CS_CHAT_MSG_CMD* packet);
	// client(chat)
	void ChatMsgCommand(SC_CHAT_MSG_CMD* packet);

public:
	// server(item)
	void ItemTotalInfoRequest(UINT32 client_index, CS_ITEM_TOTAL_INFO_REQ* packet);
	void ItemUnequipRequest(UINT32 client_index, CS_ITEM_UNEQUIP_REQ* packet);
	void ItemEquipRequest(UINT32 client_index, CS_ITEM_EQUIP_REQ* packet);
	void ItemMoveRequest(UINT32 client_index, CS_ITEM_MOVE_REQ* packet);
	void ItemCreateRequest(UINT32 client_index, CS_ITEM_CREATE_REQ* packet);
	void ItemUseRequest(UINT32 client_index, CS_ITEM_USE_REQ* packet);

	// client(item)
	void ItemTotalInfoResponse(CS_ITEM_TOTAL_INFO_RES* packet);
	void ItemUnEquipResponse(CS_ITEM_UNEQUIP_RES* packet);
	void ItemEquipResponse(CS_ITEM_EQUIP_RES* packet);
	void ItemMoveResponse(CS_ITEM_MOVE_RES* packet);
	void ItemCreateResponse(CS_ITEM_CREATE_RES* packet);
	void ItemUseResponse(CS_ITEM_USE_RES* packet);
	void ItemInfoCommand(SC_ITEM_INFO_CMD* packet);


private:
	UPROPERTY() class UAZGameInstance* game_instance_;
};
