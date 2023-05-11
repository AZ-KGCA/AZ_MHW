// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameMode/AZGameMode_Login.h"

#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/HUD/AZHUD_Login.h"
#include "AZ_MHW/Manager/AZGameOption.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "Kismet/GameplayStatics.h"


AAZGameMode_Login::AAZGameMode_Login()
{
	//static ConstructorHelpers::FClassFinder<AController> PlayerControllerPClass(TEXT("/Game/AZ/Widget/BP_DefaultController"));

	game_mode_flag_ = EGameModeFlag::Login;

	DefaultPawnClass = nullptr;
	//PlayerControllerClass = PlayerControllerPClass.Class;
	HUDClass = AAZHUD_Login::StaticClass();
}

void AAZGameMode_Login::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);

	AZGameInstance->game_option->InitGameOption();
}
