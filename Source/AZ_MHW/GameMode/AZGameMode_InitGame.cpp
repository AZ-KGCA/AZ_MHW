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
	
	//Player처리하기
	//연결된 클라이언트의 플레이어 컨트롤러로 부터 입력값을 전송받는다.
	//입력값을 전송받은 후 그것을 동일한 플레이어의 클론 캐릭터에 전달한다.(결국 플레이어블을 4개 가지고 플레이하는것과 같다.)
	
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


