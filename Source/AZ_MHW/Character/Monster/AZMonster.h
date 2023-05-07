// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonster.generated.h"

// Forward declaration of classes
class UAIPerceptionStimuliSourceComponent;
class UAZAnimInstance_Monster;
class UAZMonsterAggroComponent;
class UAZMonsterHealthComponent;
class UAZMonsterMeshComponent;

// Forward declaration of enum classes
enum class EMoveState : uint8;
enum class EMonsterActionMode : uint8;
enum class EEffectDurationType : uint8;
enum class EBossRank : uint8;
enum class EMonsterBehaviorType : uint8;

//TODO
// Delegates
//DECLARE_DELEGATE_OneParam(FOnDamaged, int32, damage_amount);

UCLASS(Blueprintable)
class AZ_MHW_API AAZMonster : public AAZCharacter
{
	GENERATED_BODY()
	
public:
	AAZMonster();
	
	// Overrides
	virtual void PreInitializeComponents() override;
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& out_location, FRotator& out_rotation) const override;
		
	// Property Initialisers
	void SetUpStimulus();
	void SetUpDefaultProperties();
	void SetMonsterInfo();
	void SetBossInfo();
	void SetActionInfo();
	void InitializeRuntimeValues();
	
	// State Setters
	void EnterCombat();
	void SetActionMode(EMonsterActionMode action_mode);
	UFUNCTION(BlueprintCallable) void SetMoveState(EMoveState move_state);
	UFUNCTION(BlueprintCallable) void SetTargetAngle(float angle);
	void SetActionState(int32 action_id);
	void ResetTargetAngle();

	// Property Getters
	int32 GetMonsterID() const;
	EBossRank GetBossRank() const;
	EMonsterBehaviorType GetBehaviorType() const;

	// State Checkers
	bool IsInCombat() const;
	bool IsFlying() const;
	bool IsMoving() const;
	
	// AnimNotifyHandlers
	virtual void AnimNotify_EndOfAction() override;
	virtual void AnimNotify_JumpToAnimation(FName next_animation_name, FName next_montage_section_name) override;
	virtual void AnimNotify_SetMovementMode(EMovementMode movement_mode) override;
	virtual void AnimNotify_DoSphereTrace(FName socket_name, float radius, EEffectDurationType duration_type, float duration) override;

	// StateReceiveHandlers

	// Validity Checkers
	bool IsABoss() const;
	bool IsAValidMonster() const;
	
public:
	// BlueprintReadWrite FOR DEBUG ONLY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AZ | Monster | States") FMonsterActionStateInfo action_state_info_;
	
	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAZMonsterAggroComponent> aggro_component_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAZMonsterHealthComponent> health_component_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAZMonsterMeshComponent> mesh_component_;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Components") TObjectPtr<UAIPerceptionStimuliSourceComponent> stimulus_component_;

	// AIController Properties: Defined from table
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") FMonsterSightConfigs sight_configs_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") int32 patrol_range_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") float patrol_delay_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") int32 percept_radius_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | AIController") FName behavior_tree_filename_;
	UPROPERTY(EditAnywhere, Category = "AZ | Monster | AIController") float acceptance_radius_;

	// Animation: Defined from table
	UPROPERTY(VisibleAnywhere, Category = "AZ | Animation")	TMap<int32, FMonsterCombatActionInfo> combat_action_map_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Animation")	TMap<int32, FMonsterNonCombatActionInfo> noncombat_action_map_;
	
protected:
	// Monster base information set from blueprint
	UPROPERTY(EditAnywhere, Category = "AZ | Monster | Properties") int32 monster_id_;
	UPROPERTY(EditAnywhere, Category = "AZ | Boss") EBossRank rank_;
	
	// Properties: Defined from table
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | Properties") EMonsterBehaviorType behavior_type_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Boss") int32 boss_id_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Boss") bool has_combat_transition_anim_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Boss") FBossRageStats rage_stats_;
	
	// Properties: Defined runtime
	UPROPERTY(EditAnywhere, Category = "AZ | Monster | States") bool is_flying_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Monster | States") bool is_in_ragemode_;

	// Other properties
	UPROPERTY(VisibleAnywhere, Category = "AZ | Animation") UAZAnimInstance_Monster* anim_instance_;

	// Event
	//DECLARE_EVENT(AAZMonster, FEnterCombatEvent) FEnterCombatEvent OnEnterCombatEvent;
	DECLARE_EVENT(AAZMonster, FEnterRageModeEvent) FEnterRageModeEvent OnEnterRageModeEvent; //TODO
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZ | Monster | Others")
	TArray<TEnumAsByte<EObjectTypeQuery>> hit_object_types_;
};

/*
 *	TODO: Check validity of new action state in monster class, not animation instance 
 *		move related variables and functions
*/