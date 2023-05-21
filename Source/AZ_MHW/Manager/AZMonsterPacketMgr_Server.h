// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AZMonsterPacketMgr_Server.generated.h"

class AAZMonster;

UCLASS()
class AZ_MHW_API UAZMonsterPacketMgr_Server : public UObject
{
	GENERATED_BODY()

private:
	AAZMonster* GetMonster(int32 monster_serial);
	
public:
	// Server to Client
	void Send_FCG_MONSTER_TRANSFORM_CMD(bool is_forced);
	void Send_FCG_MONSTER_BODY_STATE_CMD();
	void Send_FCG_MONSTER_ENTER_COMBAT_CMD();
	void Send_FCG_MONSTER_ACTION_START_CMD(float start_position = 0.0f);
	void Send_FCG_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type);
	void Send_FCG_PLAYER_ATTACK_HIT_CMD(int32 attacker_serial, FVector hit_pos, int32 damage_amount);

private:
	TMap<int32, AAZMonster*> monster_map_;
};
