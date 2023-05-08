// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimNotifies/AnimNotify.h>
#include "AZAnimNotify_ChangeMontage.generated.h"

/**
 * 해당 노티파이가 실행되면 몽타주 혹은 섹션을 변경합니다.
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_ChangeMontage : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAZAnimNotify_ChangeMontage();

	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere)
	FName target_montage_name_;

	UPROPERTY(EditAnywhere)
	FName target_section_name_;
};
