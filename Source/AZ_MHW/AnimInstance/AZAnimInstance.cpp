// Fill out your copyright notice in the Description page of Project Settings.

<<<<<<< HEAD
#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/AZ_MHW.h"
=======

#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
>>>>>>> origin/feature/Character

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

void UAZAnimInstance::NativeUpdateAnimation(float delta_seconds)
{
<<<<<<< HEAD
	Super::NativeUpdateAnimation(DeltaSeconds);
=======
	Super::NativeUpdateAnimation(delta_seconds);
	
>>>>>>> origin/feature/Character
}
