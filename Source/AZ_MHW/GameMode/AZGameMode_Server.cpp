// Copyright Team AZ. All Rights Reserved.


#include "AZGameMode_Server.h"
#include "AZ_MHW/PlayerController/AZPlayerController_Server.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"


// Sets default values
AAZGameMode_Server::AAZGameMode_Server()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	bNetLoadOnClient = false;
	bPauseable = true;
	bStartPlayersAsSpectators = false;

	//Server용 폰만들기?
	DefaultPawnClass = ADefaultPawn::StaticClass();
	
	//Server용 플레이어 컨트롤러
	PlayerControllerClass = AAZPlayerController_Server::StaticClass();
	
	//Player처리하기
	//연결된 클라이언트의 플레이어 컨트롤러로 부터 입력값을 전송받는다.
	//입력값을 전송받은 후 그것을 동일한 플레이어의 클론 캐릭터에 전달한다.(결국 플레이어블을 4개 가지고 플레이하는것과 같다.)
	
	PlayerStateClass = APlayerState::StaticClass();
	GameStateClass = AGameStateBase::StaticClass();
	HUDClass = AHUD::StaticClass();
	//GameSessionClass = AGameSession::StaticClass();
	//SpectatorClass = ASpectatorPawn::StaticClass();

	//ReplaySpectatorPlayerControllerClass = APlayerController::StaticClass();
	//ServerStatReplicatorClass = AServerStatReplicator::StaticClass();
}

// Called when the game starts or when spawned
void AAZGameMode_Server::BeginPlay()
{
	Super::BeginPlay();
	
}


