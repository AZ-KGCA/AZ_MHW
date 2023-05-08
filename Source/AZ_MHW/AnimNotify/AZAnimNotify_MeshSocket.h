// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_MeshSocket.generated.h"

/**
 * X소켓에 있는 에셋을 Y소켓으로 옮기는 노티파이 
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_MeshSocket : public UAnimNotify
{
	GENERATED_BODY()
public:
	UAZAnimNotify_MeshSocket();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UPROPERTY(EditAnywhere)
	USkeletalMeshSocket* ToSocket;
	UPROPERTY(EditAnywhere)
	USkeletalMeshSocket* FromSocket;
};
