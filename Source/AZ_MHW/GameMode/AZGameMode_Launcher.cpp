// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/GameMode/AZGameMode_Launcher.h"

#include "AZ_MHW/HUD/AZHUD_Launcher.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameOption.h"

AAZGameMode_Launcher::AAZGameMode_Launcher()
{
	game_mode_flag = EGameModeFlag::Launcher;

	DefaultPawnClass = nullptr;

	HUDClass = AAZHUD_Launcher::StaticClass();
}

void AAZGameMode_Launcher::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);

	AZGameInstance->game_option->InitGameOption();
}