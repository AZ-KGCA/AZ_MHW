// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsInCombat.generated.h"

class UBlackboardComponent;

UCLASS()
class AZ_MHW_API UBTDecorator_IsInCombat : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_IsInCombat();

	virtual void InitializeFromAsset(UBehaviorTree& asset) override;
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& owner_comp, uint8* node_memory) const override;
	virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& blackboard, FBlackboard::FKey changed_key_id);
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& owner_comp, uint8* node_memory) override;

#if WITH_EDITOR
	virtual FName GetNodeIconName() const override;
#endif

private:
	UPROPERTY(EditAnywhere, Category = Blackboard)
	FBlackboardKeySelector blackboard_key;
};