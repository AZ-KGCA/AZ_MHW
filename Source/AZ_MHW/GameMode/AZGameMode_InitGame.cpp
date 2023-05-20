// Copyright Team AZ. All Rights Reserved.


#include "AZGameMode_InitGame.h"

#include "AZGameMode_Server.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"


AAZGameMode_InitGame::AAZGameMode_InitGame()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bNetLoadOnClient = false;
	bPauseable = true;
	bStartPlayersAsSpectators = false;

	DefaultPawnClass = ADefaultPawn::StaticClass();
	PlayerControllerClass = APlayerController::StaticClass();
	
	PlayerStateClass = nullptr;
	GameStateClass = nullptr;
	HUDClass = nullptr;
	//GameSessionClass = AGameSession::StaticClass();
	//SpectatorClass = ASpectatorPawn::StaticClass();

	//ReplaySpectatorPlayerControllerClass = APlayerController::StaticClass();
	//ServerStatReplicatorClass = AServerStatReplicator::StaticClass();
}

// Called when the game starts or when spawned
void AAZGameMode_InitGame::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZGameMode_InitGame::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);

}

void AAZGameMode_InitGame::PlayServerMode()
{
	UGameplayStatics::OpenLevel(GetWorld(),FName("/Game/AZ/Map/PCUnitTestMap"),true,"?game=/Game/AZ/GameMode/BP_Server.BP_Server_C");
}

void AAZGameMode_InitGame::PlayClientMode()
{
	UGameplayStatics::OpenLevel(GetWorld(),FName("/Game/AZ/Map/Map_Launcher"), true, "?game=/Game/AZ/GameMode/BP_Launcher.BP_Launcher_C");
}


