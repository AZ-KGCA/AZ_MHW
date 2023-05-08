// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZAnimNotifyState.generated.h"
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
 * 애니메이션 피격면역 상태표현(기간버프)
 * 시작처리+끝남처리
 * 홀드, 캐스팅 같은
 * 지속적인 상태 확인
 */
UCLASS()
class AZ_MHW_API UAZAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()
public:
	UAZAnimNotifyState();
#pragma region Inherited function
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
#pragma endregion
public:
	//UPROPERTY(EditAnywhere) FName NameID;
};
