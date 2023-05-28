// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZAnimInstance_Player.h"
#include "AZAnimInstance_Playable.generated.h"

//DECLARE_DELEGATE_TwoParams(FOn)

/**
 * 플레이어블 애니메이션 노티파이 재생
 */
UCLASS()
class AZ_MHW_API UAZAnimInstance_Playable : public UAZAnimInstance_Player
{
	GENERATED_BODY()
	
public:
	UAZAnimInstance_Playable();

#pragma region Inherited function
protected:
	/** */
	virtual void NativeInitializeAnimation() override;
	/** */
	virtual void NativeUpdateAnimation(float delta_seconds) override;
#pragma endregion
public:

	//데이터의 처리가없는데 굳이 플레이어블에서?
	UFUNCTION() void AnimNotify_OnUseItem();
	UFUNCTION() void AnimNotify_OnGetItem();
};
