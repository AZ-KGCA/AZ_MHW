// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZAnimInstance_Monster.generated.h"

class AAZMonster;
class UBlendSpace1D;

UCLASS()
class AZ_MHW_API UAZAnimInstance_Monster : public UAZAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float delta_seconds) override;

protected:
	FORCEINLINE AAZMonster* CastMonster();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZ | Animation") TMap<FName, UBlendSpace1D*> blendspaces_;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZ | Animation") TMap<FName, UAnimMontage*> montages_;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") EMonsterActionMode action_mode_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") EMonsterActionPriority priority_score_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") EMonsterActionPriority move_type_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") bool is_tired_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") bool is_injured_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") bool is_rage_mode_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | State") float target_angle_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | StateModifier")	float rage_agility_multiplier_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | StateModifier")	float curr_play_rate_;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | AnimationState") bool is_doing_action_;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AZ | AnimationState") bool is_using_blendspace_;
};
