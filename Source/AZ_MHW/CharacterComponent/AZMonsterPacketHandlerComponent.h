﻿// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZMonsterPacketHandlerComponent.generated.h"

class AAZMonster;
class UAZGameInstance;
enum class EMonsterBodyPart : uint8;
enum class EMonsterBodyPartChangeType : uint8;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AZ_MHW_API UAZMonsterPacketHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAZMonsterPacketHandlerComponent();
	void Init();
	virtual void BeginPlay() override;

	// Server to Client
	void Send_SC_MONSTER_SPAWN_CMD(UINT32 newly_joined_client_idx);
	void Send_SC_MONSTER_TRANSFORM_CMD(bool is_forced = true, UINT32 newly_joined_client_idx = 0);
	void Send_SC_MONSTER_BODY_STATE_CMD(UINT32 newly_joined_client_idx);
	void Send_SC_MONSTER_ENTER_COMBAT_CMD();
	void Send_SC_MONSTER_ACTION_START_CMD(float start_position = 0.0f, UINT32 newly_joined_client_idx = 0);
	void Send_SC_MONSTER_ACTION_END_CMD();
	void Send_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type);
	void Send_SC_MONSTER_HIT_CMD(FVector hit_pos, int32 damage_amount, UINT32 attacker_client_idx);
	void Send_SC_MONSTER_DIE_CMD();

	// Client to Server
	void Receive_CS_MONSTER_UPDATE_REQ(UINT32 client_index);

	// Delegate binds
	void OnBodyPartSevered(EMonsterBodyPart body_part);
	
private:
	TWeakObjectPtr<AAZMonster> owner_;
	TWeakObjectPtr<UAZGameInstance> game_instance_;
};