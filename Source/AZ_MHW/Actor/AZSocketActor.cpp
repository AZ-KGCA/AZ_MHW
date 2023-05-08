// Fill out your copyright notice in the Description page of Project Settings.


#include "AZSocketActor.h"


// Sets default values
AAZSocketActor::AAZSocketActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	SocketObject = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SocketItem"));
	SetRootComponent(SocketObject);

	SocketObject->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AAZSocketActor::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZSocketActor::SetSocketComponent(FName SocketName, USceneComponent* Parent)
{
	if(Parent == nullptr)
	{
		if(SocketParentActor == nullptr)
		{
			//this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			this->AttachToComponent(SocketParentActor, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
			CurrentSocketName = SocketName;
		}
	}
	else
	{
		//this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		this->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
		CurrentSocketName = SocketName;
	}
}
