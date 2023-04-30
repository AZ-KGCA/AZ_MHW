// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZAnimInstance_Monster.generated.h"

class AAZMonster;
class UBlendSpace1D;

UCLASS()
class AZ_MHW_API UAZAnimInstance_Monster : public UAZAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

protected:
	FORCEINLINE AAZMonster* CastMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation) TMap<FName, UBlendSpace1D*> blendspaces_;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation) TMap<FName, UAnimMontage*> montages_;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State) EMonsterActionMode action_mode_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State) EMonsterActionPriority priority_score_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State) EMonsterActionPriority move_type_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State) bool is_tired_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State) bool is_ill_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State) bool is_rage_mode_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = State) float target_angle_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = StateModifier)	float rage_agility_multiplier_;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationState) bool is_doing_action_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AnimationState) bool is_using_blendspace_;
};
