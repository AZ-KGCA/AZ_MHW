// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_JumpToAnimation.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_JumpToAnimation : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAZAnimNotify_JumpToAnimation();
	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName next_animation_name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FName next_montage_section_name;
};
