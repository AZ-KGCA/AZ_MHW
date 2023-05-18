// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_Server.h"


// Sets default values
AAZPlayerController_Server::AAZPlayerController_Server()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AAZPlayerController_Server::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZPlayerController_Server::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void AAZPlayerController_Server::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
}
