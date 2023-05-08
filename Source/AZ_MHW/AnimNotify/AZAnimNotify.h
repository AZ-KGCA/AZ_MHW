// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
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
 * 즉발처리
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAZAnimNotify();
#pragma region Inherited function
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

#pragma endregion
public:
	//UPROPERTY(EditAnywhere) FName NameID;
};
