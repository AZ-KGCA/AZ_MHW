// Fill out your copyright notice in the Description page of Project Settings.

#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/AZ_MHW.h"

UAZAnimInstance::UAZAnimInstance()
{
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> EXAMPLE_MONTAGE(TEXT("/Game/"));
	//if(EXAMPLE_MONTAGE.Succeeded()) { //=EXAMPLE_MONTAGE.Object; }
}

void UAZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	owner_ = Cast<AAZCharacter>(TryGetPawnOwner());
}

void UAZAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
