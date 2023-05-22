// Copyright Team AZ. All Rights Reserved.


#include "AZGameMode_InitGame.h"
#include <GameFramework/DefaultPawn.h>
#include <GameFramework/GameStateBase.h>
#include <Kismet/GameplayStatics.h>

#include "GameFramework/GameState.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"


AAZGameMode_InitGame::AAZGameMode_InitGame()
{
	PrimaryActorTick.bCanEverTick = false;
	bStartPlayersAsSpectators = false;
	bNetLoadOnClient = false;
	bPauseable = true;

	DefaultPawnClass = ADefaultPawn::StaticClass();//기본
	PlayerControllerClass = APlayerController::StaticClass();//기본
	
	PlayerStateClass = APlayerState::StaticClass();//분기
	GameStateClass = AGameState::StaticClass();//분기
	HUDClass = AHUD::StaticClass();//분기
	
	//GameSessionClass = AGameSession::StaticClass();
	//SpectatorClass = ASpectatorPawn::StaticClass();

	//ReplaySpectatorPlayerControllerClass = APlayerController::StaticClass();
	//ServerStatReplicatorClass = AServerStatReplicator::StaticClass();
}

void AAZGameMode_InitGame::BeginPlay()
{
	Super::BeginPlay();

	/*
	const FString mode_string = FCommandLine::Get();
	
	if(mode_string == "") PlayServerMode();
	else PlayClientMode();
	*/
}

void AAZGameMode_InitGame::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);

}

void AAZGameMode_InitGame::PlayServerMode()
{
	//FIXME(빌드시 동기화할 맵으로 변경)
	UGameplayStatics::OpenLevel(GetWorld(),FName("/Game/AZ/Map/PCUnitTestMap"),true,"?game=/Game/AZ/GameMode/BP_DevServer.BP_DevServer_C");
	//UGameplayStatics::OpenLevel(GetWorld(),FName("/Game/AZ/Map/Map_InGame"),true,"?game=/Game/AZ/GameMode/BP_Server.BP_Server_C");
}

void AAZGameMode_InitGame::PlayClientMode()
{
	UGameplayStatics::OpenLevel(GetWorld(),FName("/Game/AZ/Map/PCUnitTestMap"), true, "?game=/Game/AZ/GameMode/BP_DevLogin.BP_DevLogin_C");
	//UGameplayStatics::OpenLevel(GetWorld(),FName("/Game/AZ/Map/Map_Launcher"), true, "?game=/Game/AZ/GameMode/BP_DevLogin.BP_DevLogin_C");
}


