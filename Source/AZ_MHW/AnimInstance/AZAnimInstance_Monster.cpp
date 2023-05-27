// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimInstance/AZAnimInstance_Monster.h"

#include "Character/Monster/AZMonster.h"
#include "Character/Monster/AZMonster_Client.h"

void UAZAnimInstance_Monster::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UAZAnimInstance_Monster::NativeUpdateAnimation(float delta_seconds)
{
	Super::NativeUpdateAnimation(delta_seconds);
}

AAZMonster* UAZAnimInstance_Monster::GetMonster()
{
	if (owner_.Get())
	{
		return Cast<AAZMonster>(owner_);
	}
	return nullptr;
}

AAZMonster_Client* UAZAnimInstance_Monster::GetMonsterClient()
{
	if (owner_.Get())
	{
		return Cast<AAZMonster_Client>(owner_);
	}
	return nullptr;
}
