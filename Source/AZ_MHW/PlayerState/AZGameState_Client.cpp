// Copyright Team AZ. All Rights Reserved.


#include "AZGameState_Client.h"


// Sets default values
AAZGameState_Client::AAZGameState_Client()
{
	PrimaryActorTick.bCanEverTick = false;
	
}

// Called when the game starts or when spawned
void AAZGameState_Client::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZGameState_Client::BeginDestroy()
{
	Super::BeginDestroy();
}

