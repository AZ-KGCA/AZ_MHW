// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_FindTarget.generated.h"

class UBehaviorTree;
/**
 * 
 */
UCLASS()
class AZ_MHW_API UBTService_FindTarget : public UBTService
{
	GENERATED_BODY()

	UBTService_FindTarget(const FObjectInitializer& object_initializer);
	
	virtual void InitializeFromAsset(UBehaviorTree& asset) override;
	virtual void TickNode(UBehaviorTreeComponent& owner_comp, uint8* node_memory, float delta_seconds) override;

protected:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector blackboard_key;
};
