// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonster.generated.h"

class UAIPerceptionStimuliSourceComponent;
class UAZAnimInstance_Monster;
class UAZAggroComponent;
class UAZMonsterHealthComponent;
enum class EMoveState : uint8;
enum class EMonsterActionMode : uint8;
enum class EEffectDurationType : uint8;
enum class EBossRank : uint8;


UCLASS(Blueprintable)
class AZ_MHW_API AAZMonster : public AAZCharacter
{
	GENERATED_BODY()
	
public:
	AAZMonster();

	// Overrides
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	// Property Initialisers
	void SetUpStimulus();
	void SetUpDefaultProperties();
	void SetMonsterInfo();
	void SetBossInfo();
	void SetActionInfo();
	
	// State Setters
	void EnterCombatMode();
	UFUNCTION(BlueprintCallable) void SetActionMode(EMonsterActionMode action_mode);
	UFUNCTION(BlueprintCallable) void SetMoveState(EMoveState move_state);
	UFUNCTION(BlueprintCallable) void SetTargetAngle(float angle);
	UFUNCTION(BlueprintCallable) void SetActionState(int32 action_id);
	
	// AnimNotifyHandlers
	virtual void AnimNotify_EndOfAction() override;
	virtual void AnimNotify_JumpToAnimation(FName next_animation_name, FName next_montage_section_name) override;
	virtual void AnimNotify_SetMovementMode(EMovementMode movement_mode) override;
	virtual void AnimNotify_DoSphereTrace(FName socket_name, float radius, EEffectDurationType duration_type, float duration) override;

	// StateReceiveHandlers
	
public:
	// Monster id set from blueprint
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster | Properties") int32 monster_id_;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss") EBossRank rank_;
	
	// Properties: Defined from table
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | Properties") EMonsterBehaviorType behavior_type_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss") int32 boss_id_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss") bool has_combat_transition_anim_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss") FBossRageStats rage_stats_;

	// AIController Properties: Defined from table
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | AIController") FMonsterSightConfigs sight_configs_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | AIController") int32 patrol_range_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | AIController") float patrol_delay_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | AIController") int32 percept_radius_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | AIController") FName behavior_tree_filename_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | AIController") FName blackboard_data_filename_;

	// Properties: Defined runtime
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | States") bool is_flying_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | States") bool is_in_rage_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Monster | States") FBossMonsterStateInfo state_info_;

	// Other properties
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation") UAZAnimInstance_Monster* anim_instance_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | Components") UAZAggroComponent* aggro_component_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Monster | Components") UAZMonsterHealthComponent* health_component_;

	// Animation: Defined from table
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<int32, FMonsterCombatActionInfo> combat_action_map_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Animation")
	TMap<int32, FMonsterNonCombatActionInfo> noncombat_action_map_;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Monster | Others")
	TArray<TEnumAsByte<EObjectTypeQuery>> hit_object_types_;

private:
	UAIPerceptionStimuliSourceComponent* stimulus_;
};
