// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AZAnimInstance.generated.h"

class AAZCharacter;

UCLASS(abstract)
class AZ_MHW_API UAZAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = AZ)
	TWeakObjectPtr<AAZCharacter> owner_;
};
