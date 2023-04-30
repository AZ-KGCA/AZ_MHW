// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindRandomLocation.generated.h"

enum class EGoalSearchStyle : uint8;

UCLASS(Blueprintable)
class AZ_MHW_API UBTTask_FindRandomLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
// public:
// 	UBTTask_FindRandomLocation();
//
// private:
// 	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);
//
// private:
// 	UPROPERTY(EditAnywhere, Category = "Search")
// 	EGoalSearchStyle SearchStyle;
};
