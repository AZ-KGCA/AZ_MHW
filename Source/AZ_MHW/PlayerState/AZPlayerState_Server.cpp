﻿// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerState_Server.h"


AAZPlayerState_Server::AAZPlayerState_Server()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAZPlayerState_Server::BeginPlay()
{
	Super::BeginPlay();
}

void AAZPlayerState_Server::BeginDestroy()
{
	Super::BeginDestroy();
}

