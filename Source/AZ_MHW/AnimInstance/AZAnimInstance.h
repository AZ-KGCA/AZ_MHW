// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimInstance.h>
#include "AZAnimInstance.generated.h"
 
/**
 * ABP 부모클래스의 Base Class
 * AnimInstance는 AnimClass + AnimGraph의 구조를 이루며,
 * AnimClass는 AnimGraph가 사용할 데이터와, 애니메이션 콜백을 정의
 * 
 * 모든 일반 블루프린터업데이트가 호출된 후에,
 * AnimGraph(BluePrint만 작성이 가능한 애니메이션)가 호출되어
 * 애니메이션(최종포즈)을 실행한다.
 */
UCLASS()
class AZ_MHW_API UAZAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UAZAnimInstance();
	
protected:
	/**
	 * 애님 인스턴스 캡쳐 업데이트
	 * 애니메이션 실행 조건값 동기화용도
	 */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
