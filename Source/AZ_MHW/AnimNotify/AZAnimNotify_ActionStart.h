// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_ActionStart.generated.h"

/**
 * 몬타주 섹션의 시작을 알림.
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_ActionStart : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAZAnimNotify_ActionStart();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	UPROPERTY(EditAnywhere)
	FName ActionName;
	UPROPERTY(EditAnywhere)
	FName StateName;
};
