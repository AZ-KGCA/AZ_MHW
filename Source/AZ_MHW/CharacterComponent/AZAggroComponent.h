// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZAggroComponent.generated.h"
class AAZCharacter;
class AAZMonster;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZAggroComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAZAggroComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:	
	int32 GetBestTargetSerial();
	AAZCharacter* GetRefToTarget() const;
	
	FVector GetTargetLocation() const;
	float GetDistance2DToTarget() const;

private:
	TWeakObjectPtr<AAZMonster> owner_;
	TWeakObjectPtr<AAZCharacter> target_;

private:
	TMap<int32, int32> aggro_info_;
};