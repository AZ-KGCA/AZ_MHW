// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimNotifies/AnimNotify.h>
#include "AZAnimNotify_InputAction.generated.h"

/**
 * 해당 노티파이가 실행되면 입력값을 비트마스크로 비교하여 처리합니다.
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_InputAction : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAZAnimNotify_InputAction();

	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere)
	int32 bitmask;

	UPROPERTY(EditAnywhere)
	FName montage_name;

	UPROPERTY(EditAnywhere)
	FName section_name;
};
