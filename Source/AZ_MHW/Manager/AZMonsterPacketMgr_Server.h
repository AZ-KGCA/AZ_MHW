// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AZMonsterPacketMgr_Server.generated.h"

class AAZMonster;
enum class EMonsterBodyPart : uint8;
enum class EMonsterBodyPartChangeType : uint8;

UCLASS()
class AZ_MHW_API UAZMonsterPacketMgr_Server : public UObject
{
	GENERATED_BODY()

private:
	AAZMonster* GetMonster(int32 monster_serial);
	
public:
	void AddMonster(AAZMonster* monster);
	
	// Server to Client
	void Send_FCG_MONSTER_SPAWN_CMD(int32 monster_serial);
	void Send_FCG_MONSTER_TRANSFORM_CMD(int32 monster_serial, bool is_forced);
	void Send_FCG_MONSTER_BODY_STATE_CMD(int32 monster_serial);
	void Send_FCG_MONSTER_ENTER_COMBAT_CMD(int32 monster_serial);
	void Send_FCG_MONSTER_ACTION_START_CMD(int32 monster_serial, float start_position = 0.0f);
	void Send_FCG_MONSTER_PART_CHANGE_CMD(int32 monster_serial, EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type);
	void Send_FCG_PLAYER_ATTACK_HIT_CMD(int32 monster_serial, int32 attacker_serial, FVector hit_pos, int32 damage_amount);

private:
	UPROPERTY() TMap<int32, AAZMonster*> monster_map_;
	int32 next_monster_serial_;
};
