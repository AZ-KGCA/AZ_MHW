// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/AZ_MHW.h"

void UAZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	owner_ = Cast<AAZCharacter>(TryGetPawnOwner());
}
