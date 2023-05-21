// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AZMonsterPacketMgr_Client.generated.h"

class AAZMonster_Client;

UCLASS()
class AZ_MHW_API UAZMonsterPacketMgr_Client : public UObject
{
	GENERATED_BODY()

private:
	AAZMonster_Client* GetMonster(int32 monster_serial);
	
public:
	// Server to Client
	void Receive_FCG_MONSTER_TRANSFORM_CMD(const struct FCG_MONSTER_TRANSFORM_CMD& recv_msg);
	void Receive_FCG_MONSTER_BODY_STATE_CMD(const struct FCG_MONSTER_BODY_STATE_CMD& recv_msg);
	void Receive_FCG_MONSTER_ENTER_COMBAT_CMD(const struct FCG_MONSTER_ENTER_COMBAT_CMD& recv_msg);
	void Receive_FCG_MONSTER_ACTION_START_CMD(const struct FCG_MONSTER_ACTION_START_CMD& recv_msg);
	void Receive_FCG_MONSTER_PART_CHANGE_CMD(const struct FCG_MONSTER_PART_CHANGE_CMD& recv_msg);
	void Receive_FCG_PLAYER_ATTACK_HIT_CMD(const struct FCG_PLAYER_ATTACK_HIT_CMD& recv_msg);

private:
	TMap<int32, AAZMonster_Client*> monster_map_;	
};
