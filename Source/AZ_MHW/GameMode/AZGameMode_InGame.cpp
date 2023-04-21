// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/AZGameMode_InGame.h"

#include "Character/Player/AZPlayer_Playable.h"
#include "PlayerController/AZPlayerController_InGame.h"
#include "PlayerState/AZPlayerState.h"

AAZGameMode_InGame::AAZGameMode_InGame()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/AZ/Character/BluePrint/BP_AZPlayer_Playable"));
	static ConstructorHelpers::FClassFinder<AController> PlayerControllerPClass(TEXT("/Game/AZ/Character/BluePrint/BP_AZPlayerController_InGame"));

	DefaultPawnClass = PlayerPawnBPClass.Class;//AAZPlayer_Playable::StaticClass();//
	PlayerControllerClass = PlayerControllerPClass.Class;//AAZPlayerController_InGame::StaticClass();
	PlayerStateClass = AAZPlayerState::StaticClass();
	//GameStateClass =
	//HUDClass =
	//SpectatorClass =
}
void AAZGameMode_InGame::PostLogin(APlayerController* NewPlayer)
{
	//Server
	Super::PostLogin(NewPlayer);
}