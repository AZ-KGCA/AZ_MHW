// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AZSocketActor.generated.h"

UCLASS()
class AZ_MHW_API AAZSocketActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAZSocketActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* SocketObject;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* SocketParentActor;
	
	FName CurrentSocketName;

	void SetSocketComponent(FName SocketName, USceneComponent* Parent = nullptr);
	//아이템로드
	//아이템언로드
	//소켓옮기기
};
//auto SocketItem = GetWorld()->SpawnActor<AAZSocketActor>(FVector::ZeroVector,FRotator::ZeroRotator);
//if(SocketItem != nullptr) SocketItem->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);