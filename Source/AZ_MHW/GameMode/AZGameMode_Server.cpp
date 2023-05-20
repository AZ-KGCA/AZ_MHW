// Copyright Team AZ. All Rights Reserved.


#include "AZGameMode_Server.h"
#include "AZ_MHW/PlayerController/AZPlayerController_Server.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "PlayerState/AZGameState_Server.h"
#include "PlayerState/AZPlayerState_Server.h"


AAZGameMode_Server::AAZGameMode_Server()
{
	PrimaryActorTick.bCanEverTick = true;

	bNetLoadOnClient = false;
	bPauseable = true;
	bStartPlayersAsSpectators = false;

	DefaultPawnClass = ADefaultPawn::StaticClass();
	//HUDClass = AHUD::StaticClass();//Server UI
	
	PlayerControllerClass = AAZPlayerController_Server::StaticClass();//Server Controller
	PlayerStateClass = AAZPlayerState_Server::StaticClass();//Server PlayerState
	GameStateClass = AAZGameState_Server::StaticClass();//Server GameState
	
	//GameSessionClass = AGameSession::StaticClass();
	//SpectatorClass = ASpectatorPawn::StaticClass();
	//ReplaySpectatorPlayerControllerClass = APlayerController::StaticClass();
	//ServerStatReplicatorClass = AServerStatReplicator::StaticClass();
}

void AAZGameMode_Server::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZGameMode_Server::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}


