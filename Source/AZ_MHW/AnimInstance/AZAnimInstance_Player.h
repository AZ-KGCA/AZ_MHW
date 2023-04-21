// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AnimInstance/AZAnimInstance.h"
#include "AZAnimInstance_Player.generated.h"
#pragma region // Macro

#pragma endregion
#pragma region // TypeDefine
DECLARE_MULTICAST_DELEGATE(FOnActionStartDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEnableInputDelegate);
DECLARE_MULTICAST_DELEGATE(FOnEnableActionDelegate);
DECLARE_MULTICAST_DELEGATE(FOnActionEndDelegate);

DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnCastingCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnDisableCheckDelegate);
#pragma endregion 
#pragma region //ForwardDeclaration
class AAZPlayerState;
#pragma endregion
#pragma region //Enum

#pragma endregion 
#pragma region //Struct

#pragma endregion
/**
 * 플레이어 애니메이션블루프린트의 뼈대
 * 상태별(일반), 무기(무기)별 ABP(AnimBluePrint)로 생성
 * 상태값으로 애니메이션을 실행
 */
UCLASS()
class AZ_MHW_API UAZAnimInstance_Player : public UAZAnimInstance
{
	GENERATED_BODY()

public:
	UAZAnimInstance_Player();

#pragma region //Inherited function
protected:
	/** */
	virtual  void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion
public:
	/** 몬스터나 NPC도 사용하게 부모로 옮길 것*/
	/** 애니메이션 실행시 1번*/
	UFUNCTION() void AnimNotify_OnActionStart();//액션 시작. 입력비활성화, 액션비활성화
	FOnActionStartDelegate OnActionStart;
	
	UPROPERTY(BlueprintReadOnly)
	uint32 bCanAction:1;
	/** 애니메이션 입력값 받기 2번*/
	UFUNCTION() void AnimNotify_OnEnableInput();//미리 입력할 수 있는 값을 조건
	FOnEnableInputDelegate OnEnableInput;
	/** 애니메이션 입력 가능 3번*/
	UFUNCTION() void AnimNotify_OnEnableAction();//연계입력시 연계액션가능시점
	FOnEnableActionDelegate OnEnableAction;
	/** 애니메이션 종료(몽타주) 4번*/
	UFUNCTION() void AnimNotify_OnActionEnd();//액션이 끝났으니 처음부터
	FOnActionEndDelegate OnActionEnd;
	/** 애니메이션 공격 체크*/
	UFUNCTION() void AnimNotify_OnAttackHitCheck();
	FOnAttackHitCheckDelegate OnAttackHitCheck;
	/** 애니메이션 캐스팅 체크
	 * 
	 */
	UFUNCTION() void AnimNotify_OnCastingCheck();
	FOnAttackHitCheckDelegate OnCastingCheck;
	/** 애니메이션 상태이상 해제 체크*/
	UFUNCTION() void AnimNotify_OnDisableCheck();
	FOnAttackHitCheckDelegate OnDisableCheck;

	
	UPROPERTY(BlueprintReadOnly)
	uint32 bIsHit:1;
	
	
	UPROPERTY(BlueprintReadWrite)
	AAZPlayerState* AZPlayerStateCache;
	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* CurrentMontage;
	UPROPERTY(BlueprintReadWrite)
	FName NextMontageSectionName;
	
	UPROPERTY(BlueprintReadOnly)
	TMap<FName, UAnimMontage*> MontageMap;
	








	
public://C++로 옮길것
	/** 플레이어 입력값으로 방향체크 */
	UFUNCTION(BlueprintCallable)
	FVector GetVelocity(bool Forward, bool Back, bool Left, bool Right) const;

	
public://블루 프린트
	/** 디자이너용 비트마스크 생성
	 * 테이블로 뺄수도 있음
	 */

	//FName(현재 섹션 + 입력 = 액션)
	//UPROPERTY() TMap<FName, int32> ActionBitMaskMap;
};