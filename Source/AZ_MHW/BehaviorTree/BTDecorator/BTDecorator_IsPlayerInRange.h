// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsPlayerInRange.generated.h"


UCLASS()
class AZ_MHW_API UBTDecorator_IsPlayerInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsPlayerInRange();

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& owner_comp, uint8* node_memory) const override;
#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

public:
	UPROPERTY(EditAnywhere) float percept_radius;
};
