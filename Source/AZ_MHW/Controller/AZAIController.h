// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AIController.h"
#include "AZAIController.generated.h"

class UBehaviorTreeComponent;
class UBehaviorTree;
class UAISenseConfig_Sight;
class AAZMonster;
enum class ECharacterState : uint8;

UCLASS(Blueprintable)
class AZ_MHW_API AAZAIController : public AAIController
{
	GENERATED_BODY()

public:
	AAZAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* const pawn) override;
	
private:
	// Sight perception
	UFUNCTION() void SetUpPerceptionSystem();
	UFUNCTION() void OnPlayerDetected(AActor* actor, FAIStimulus const stimulus);

protected:
	// Team Agent Interface overrides
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& other_actor) const override;

public:
	// Set up default properties
	void SetUpProperties();
	void SetUpBehaviorTree();
		
	// Blackboard value setter functions
	void SetBlackboardValueAsFloat(FName bb_key_name, float bb_key_value);
	void SetBlackboardValueAsBool(FName bb_key_name, bool bb_key_value);
	void SetBlackboardValueAsEnum(FName bb_key_name, uint8 enum_idx);

private:
	TObjectPtr<UAISenseConfig_Sight> sight_config_;
	FGenericTeamId team_id_;
	
public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AZ") UBehaviorTree* behavior_tree_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ")	FMonsterSightConfigs sight_configs_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ")	int32 patrol_range_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ")	float patrol_delay_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ") int32 percept_radius_;
};
