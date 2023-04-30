// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZAnimInstance.generated.h"

class AAZCharacter;

UCLASS(abstract)
class AZ_MHW_API UAZAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = General)
	TWeakObjectPtr<AAZCharacter> owner_;
};
