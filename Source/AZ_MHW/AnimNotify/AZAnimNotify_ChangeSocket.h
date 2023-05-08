// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimNotifies/AnimNotify.h>
#include "AZAnimNotify_ChangeSocket.generated.h"

/**
 * 해당 노티파이가 실행되면 소켓액터의 위치를 변경합니다.
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_ChangeSocket : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAZAnimNotify_ChangeSocket();

	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

public:
	UPROPERTY(EditAnywhere)
	FName socket_actor_name_;
	//이펙트나 등등
	//소켓과 소켓사이의 선형보간기능
	
	//UPROPERTY(EditAnywhere)
	//FName from_socket_name_;
	UPROPERTY(EditAnywhere)
	FName to_socket_name_;
};
