// Fill out your copyright notice in the Description page of Project Settings.


#include "AZSocketActor.h"


// Sets default values
AAZSocketActor::AAZSocketActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SocketObject = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SocketItem"));
	RootComponent = SocketObject;
	
	SocketObject->SetCollisionProfileName(TEXT("NoCollision"));
}

// Called when the game starts or when spawned
void AAZSocketActor::BeginPlay()
{
	Super::BeginPlay();
	
}