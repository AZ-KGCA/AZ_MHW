// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZAnimInstance_Player.h"
#include "AZAnimInstance_Playable.generated.h"

//DECLARE_DELEGATE_TwoParams(FOn)

/**
 * 
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
	/** 플레이어 캐릭터 데이터 캐시*/
	UPROPERTY(BlueprintReadWrite)
	AAZPlayer* player_cache_;
	UPROPERTY(BlueprintReadWrite)
	AAZPlayerState* player_state_cache_;

	UPROPERTY(BlueprintReadWrite)
	int32 command_bit_mask_;
	/**현재 액션의 가능한 비트마스크*/
	UFUNCTION(BlueprintPure)
	int32 GetInputActionBitMask() const;//PlayerAction비트플래그로 변경
};
