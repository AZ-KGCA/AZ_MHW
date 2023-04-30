// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PatrolBegin.generated.h"

UCLASS(Blueprintable)
class AZ_MHW_API UBTTask_PatrolBegin : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_PatrolBegin();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
};
