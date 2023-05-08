// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZAnimInstance.h"
#include "AZAnimInstance_Weapon.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimInstance_Weapon : public UAZAnimInstance
{
	GENERATED_BODY()

public:
	UAZAnimInstance_Weapon();
protected:
	virtual void NativeInitializeAnimation() override;
	
	virtual void NativeUpdateAnimation(float delta_seconds) override;
public:
	//SheathWeapon 납도상태
	//DrawWeapon 발도상태
	//Animation 중간중간 애니메이션
};
