// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AZAnimInstance.h"

UAZAnimInstance::UAZAnimInstance()
{
	//static ConstructorHelpers::FObjectFinder<UAnimMontage> EXAMPLE_MONTAGE(TEXT("/Game/"));
	//if(EXAMPLE_MONTAGE.Succeeded()) { //=EXAMPLE_MONTAGE.Object; }
}

void UAZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
}

void UAZAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
}
