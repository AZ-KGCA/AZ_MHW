// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"


AAZPlayerState_Client::AAZPlayerState_Client()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAZPlayerState_Client::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZPlayerState_Client::BeginDestroy()
{
	Super::BeginDestroy();
	
}