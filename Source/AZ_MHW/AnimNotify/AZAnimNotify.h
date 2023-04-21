// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAZAnimNotify();
protected:
	//virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
