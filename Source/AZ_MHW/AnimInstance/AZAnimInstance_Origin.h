// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZAnimInstance_Player.h"
#include "AZAnimInstance_Origin.generated.h"

/**
 * 서버에서 시뮬레이션되는 플레이어의  원본 캐릭터 애님인스턴스 클래스
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimInstance_Origin : public UAZAnimInstance_Player
{
	GENERATED_BODY()
	
#pragma region //Inherited function
protected:
	/** */
	virtual void NativeInitializeAnimation() override;
	/** */
	virtual void NativeUpdateAnimation(float delta_seconds) override;
#pragma endregion
public:
	/** */
	UFUNCTION() void AnimNotify_OnUseItem();
	/** */
	UFUNCTION() void AnimNotify_OnGetItem();
	/** */
	UFUNCTION() void AnimNotify_OnGetDamage();
};
