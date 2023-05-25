// Copyright Team AZ. All Rights Reserved.
#pragma once
#include "CoreMinimal.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonster_Client.generated.h"

// Forward declaration of classes
class UAZAnimInstance_Monster;
class UAZMonsterMeshComponent_Client;

UCLASS(Blueprintable)
class AZ_MHW_API AAZMonster_Client : public AAZCharacter
{
	GENERATED_BODY()

public:
	AAZMonster_Client();
	
	// Overrides
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;

	// Property getters
	bool IsABoss() const;
	
	// Packer Handlers
	void Receive_FCG_MONSTER_TRANSFORM_CMD(const struct FCG_MONSTER_TRANSFORM_CMD& recv_msg);
	void Receive_FCG_MONSTER_BODY_STATE_CMD(const struct FCG_MONSTER_BODY_STATE_CMD& recv_msg);
	void Receive_FCG_MONSTER_ENTER_COMBAT_CMD(const struct FCG_MONSTER_ENTER_COMBAT_CMD& recv_msg);
	void Receive_FCG_MONSTER_ACTION_START_CMD(const struct FCG_MONSTER_ACTION_START_CMD& recv_msg);
	void Receive_FCG_MONSTER_PART_CHANGE_CMD(const struct FCG_MONSTER_PART_CHANGE_CMD& recv_msg);
	void Receive_FCG_PLAYER_ATTACK_HIT_CMD(const struct FCG_PLAYER_ATTACK_HIT_CMD& recv_msg);
	
	// Delegates
	DECLARE_MULTICAST_DELEGATE(FOnEnterCombatSignature);
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWoundedSignature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnWoundHealedSignature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnBrokenSigature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE_OneParam(FOnSeveredSigature, EMonsterBodyPart)
	DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
	
protected:
	// Properties
	UPROPERTY(VisibleAnywhere, Category = "AZ") int32 object_serial_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") int32 monster_id_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") int32 boss_id_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") EBossRank rank_;
	
	// States
	UPROPERTY(VisibleAnywhere) FMonsterActionStateInfo action_state_info_;
	UPROPERTY(VisibleAnywhere) bool is_dead_;

	// Components
	TObjectPtr<UAZMonsterMeshComponent_Client> mesh_component_;
	UPROPERTY(VisibleAnywhere, Category = "AZ") TObjectPtr<UAZAnimInstance_Monster> anim_instance_;

public:
	// Delegates
	FOnEnterCombatSignature OnEnterCombat;
	FOnWoundedSignature OnBodyPartWounded;
	FOnWoundHealedSignature OnBodyPartWoundHealed;
	FOnBrokenSigature OnBodyPartBroken;
	FOnSeveredSigature OnBodyPartSevered;
	FOnDeathSignature OnDeath;	
};
