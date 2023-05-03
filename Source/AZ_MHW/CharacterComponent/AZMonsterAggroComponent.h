// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZMonsterAggroComponent.generated.h"
class AAZCharacter;
class AAZMonster;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZMonsterAggroComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAZMonsterAggroComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	// Setters
	void SetBestTarget(AAZCharacter* character);
	
	// Getters
	int32 GetBestTargetSerial();
	AAZCharacter* GetTargetRef() const;
	FVector GetTargetLocation() const;
	float GetDistance2DToTarget() const;

private:
	UPROPERTY() TWeakObjectPtr<class AAZMonster> owner_;
	TWeakObjectPtr<AAZCharacter> best_target_;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TMap<int32, int32> aggro_info_;
};