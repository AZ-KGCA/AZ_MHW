// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimNotifies/AnimNotify.h>
#include "AZAnimNotify.generated.h"
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
 * 애니메이션 특정시점에 트리거되는 알림 클래스
 * 조건 처리(키입력, 상태체크)
 * 즉시 처리
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAZAnimNotify();
#pragma region Inherited function
protected:
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

#pragma endregion
public:
	//UPROPERTY(EditAnywhere) FName NameID;
};

/*
 * (조건체크후) 변수 변경
 * (조건체크후) 함수 호출
 * (조건체크후) 객체 생성
 * 
 * 카메라 흔들기
 * 투사체 생성
 * 코스트 체크
 * 커맨드 체크
 * 공격 체크//무기 끝에 선달아서 트레이스
 * 액션 시작
 * 액션 종료
 * 액션 취소
 *
 * 스킬 캐스팅 갱신
 * 스킬 캐스팅 성공
 * 스킬 캐스팅 실패
 * 스킬 홀드 시작
 * 스킬 홀드 갱신
 * 스킬 홀드 종료
 * 
 * 소켓 부착
 */