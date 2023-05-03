// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"

void UAZAnimInstance_Monster::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAZAnimInstance_Monster::NativeUpdateAnimation(float delta_seconds)
{
	Super::NativeUpdateAnimation(delta_seconds);
}

FORCEINLINE AAZMonster* UAZAnimInstance_Monster::CastMonster()
{
	return Cast<AAZMonster>(TryGetPawnOwner());
}
