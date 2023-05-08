// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZAnimInstance_Player.h"
#include "AZAnimInstance_Playable.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration

#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion
/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimInstance_Playable : public UAZAnimInstance_Player
{
	GENERATED_BODY()
	
public:
	UAZAnimInstance_Playable();

#pragma region //Inherited function
protected:
	/** */
	virtual void NativeInitializeAnimation() override;
	/** */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion
public:
	UPROPERTY()
	bool bCanRotation;//회전이 가능한지

public:
	/** 플레이어 캐릭터 캐시*/
	UPROPERTY(BlueprintReadWrite)
	AAZPlayer* AZPlayerCache;
	UPROPERTY(BlueprintReadWrite)
	AAZPlayerState* AZPlayerStateCache;

	UPROPERTY(BlueprintReadWrite)
	int32 CommandBitMask;//현재 액션의 가능한 비트마스크
	UFUNCTION(BlueprintPure)
	int32 GetInputActionBitMask() const;//PlayerAction비트플래그로 변경
};
