// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/AZ_MHW.h"

void UAZAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	owner_ = Cast<AAZCharacter>(TryGetPawnOwner());
}
