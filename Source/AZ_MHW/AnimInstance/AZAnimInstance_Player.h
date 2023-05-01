// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AnimInstance/AZAnimInstance.h"
#include "AZAnimInstance_Player.generated.h"
#pragma region // Macro

#pragma endregion
#pragma region // TypeDefine

#pragma endregion 
#pragma region //ForwardDeclaration
class AAZPlayer;
class AAZPlayerState;
#pragma endregion
#pragma region //Enum

#pragma endregion 
#pragma region //Struct

#pragma endregion
/**
 * 플레이어 애니메이션블루프린트의 뼈대
 * Controller(Input)와 Character(Event)로 변경된 State(Data)를 캐싱
 * Data를 해석해 해당 틱에 실행할 AnimGraph를 실행
 */
UCLASS(Abstract)
class AZ_MHW_API UAZAnimInstance_Player : public UAZAnimInstance
{
	GENERATED_BODY()

public:
	UAZAnimInstance_Player();

#pragma region //Inherited function
protected:
	/** */
	virtual void NativeInitializeAnimation() override;
	/** */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
#pragma endregion
public://
	/** 블루 프린트(AnimGraph)용 AnimInstance 변수
	 * 결과적으로 해당 변수들을 컨트롤 하는 것으로 애니메이션을 모두 컨트롤 할 수 있다.
	 */
	//둘다 False면 시퀀스모드
	UPROPERTY(BlueprintReadWrite)
	bool bIsBlend;//블렌드 모드
	UPROPERTY(BlueprintReadWrite)
	bool bIsMontage;//몽타주 모드
	
	UPROPERTY(BlueprintReadWrite)
	UAnimSequence* CurrentAnimSequence;//현재 애님 시퀀스
	UPROPERTY(BlueprintReadWrite)
	UBlendSpace* CurrentBlendSpace;//현재 블렌드 스페이스
	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* CurrentAnimMontage;//현재 애님 몽타주
	UPROPERTY(BlueprintReadWrite)
	float CurrentAnimPlayRate;//현재 애님 재생속도

	UPROPERTY(BlueprintReadWrite)
	FName CurrentMontageName;//현재 몽타주 이름
	UPROPERTY(BlueprintReadWrite)
	FName CurrentSectionName;//현재 섹션 이름
	UPROPERTY(BlueprintReadWrite)
	FName NextSectionName;//다음 섹션 이름

	UPROPERTY(BlueprintReadWrite)
	int32 CurrentBParam_X;//Permyriad
	UPROPERTY(BlueprintReadWrite)
	int32 CurrentBParam_Y;//Permyriad

	//종료조건(키, 트리거),지연 액션(현재 액션이 종료),강제 액션
	void SetMontage(FName MontageName);
	void SetNextMontage(FName MontageName);
	void SetSection(FName SectionName = TEXT("Default"));
	void SetNextSection(FName SectionName = TEXT("Default"));
	void SetBlendSpace(FName BlendSpaceName);//
	void SetSequence(FName SequenceName);//
	void SetPlayRate(float PlayRate = 1.5f);


	//플레이어 스테이트에 넣을만한것
	//UPROPERTY(BlueprintReadOnly) FVector StartRootPosition;//모션을 시작한 위치
	//UPROPERTY(BlueprintReadOnly) FRotator StartRootRotation;//모션을 시작한 방향
	//UPROPERTY(BlueprintReadOnly) FVector FinalRootPosition;//모션종료후 최종 위치 //Montage에서 후크
	//UPROPERTY(BlueprintReadOnly) FRotator FinalRootRotation;//모션종료후 최종 방향
};