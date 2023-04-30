// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"

void UAZAnimInstance_Monster::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

FORCEINLINE AAZMonster* UAZAnimInstance_Monster::CastMonster()
{
	return Cast<AAZMonster>(TryGetPawnOwner());
}
