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
	UAZMonsterAggroComponent();

protected:
	virtual void InitializeComponent() override;

public:
	// Setters
	void SetBestTarget(AAZCharacter* character);
	
	// Getters
	int32 GetBestTargetSerial();
	AAZCharacter* GetTargetRef() const;
	FVector GetTargetLocation() const;
	float GetDistance2DToTarget() const;
	float GetAngle2DToTarget() const;
	//TODO Get random nearby player location

private:
	TWeakObjectPtr<AAZMonster> owner_;
	TWeakObjectPtr<AAZCharacter> best_target_;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly) TMap<int32, int32> aggro_info_;
	UPROPERTY(VisibleAnywhere) int32 test_;
};