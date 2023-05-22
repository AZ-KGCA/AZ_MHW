// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CommonSource/AZStruct.h"
#include "Components/ActorComponent.h"
#include "AZMonsterPacketHandlerComponent_Client.generated.h"

class AAZMonster_Client;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AZ_MHW_API UAZMonsterPacketHandlerComponent_Client : public UActorComponent
{
	GENERATED_BODY()

public:
	UAZMonsterPacketHandlerComponent_Client();
	virtual void InitializeComponent() override;

public:
	// Handle server->client packets
	void Receive_SC_MONSTER_TRANSFORM_CMD(FVector location, FRotator rotation, bool is_forced);
	void Receive_SC_MONSTER_BODY_STATE_CMD(FBossBodyPartDebuffState head_state, FBossBodyPartDebuffState back_state,
	                                       FBossBodyPartDebuffState left_wing_state, FBossBodyPartDebuffState right_wing_state,
	                                       FBossBodyPartDebuffState tail_state);
	void Receive_SC_MONSTER_ENTER_COMBAT_CMD();
	void Receive_SC_MONSTER_ACTION_START_CMD(FMonsterActionStateInfo action_info, float start_positon);
	void Receive_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type);
	void Receive_SC_MONSTER_HIT_CMD(FVector hit_position, int32 damage_amount);
	void Receive_SC_MONSTER_DIE_CMD();

private:
	TWeakObjectPtr<AAZMonster_Client> owner_;
};
