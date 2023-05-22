// Copyright Team AZ. All Rights Reserved.


#include "AZGameMode_Server.h"
#include "AZ_MHW/PlayerController/AZPlayerController_Server.h"
#include "GameFramework/DefaultPawn.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"
#include "AZ_MHW/PlayerState/AZGameState_Server.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Server.h"
#include "AZ_MHW/Manager/AZObjectMgr_Server.h"


AAZGameMode_Server::AAZGameMode_Server()
{
	PrimaryActorTick.bCanEverTick = true;

	bNetLoadOnClient = false;
	bPauseable = true;
	bStartPlayersAsSpectators = false;

	DefaultPawnClass = ADefaultPawn::StaticClass();
	PlayerControllerClass = AAZPlayerController_Server::StaticClass();
	
	//Player처리하기
	//연결된 클라이언트의 플레이어 컨트롤러로 부터 입력값을 전송받는다.
	//입력값을 전송받은 후 그것을 동일한 플레이어의 클론 캐릭터에 전달한다.(결국 플레이어블을 4개 가지고 플레이하는것과 같다.)
	
	PlayerStateClass = AAZPlayerState_Server::StaticClass();//Server State
	GameStateClass = AAZGameState_Server::StaticClass();//Server State
	HUDClass = AHUD::StaticClass();//Server UI

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

void AAZGameMode_Server::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);
	object_mgr_ = NewObject<UAZObjectMgr_Server>();
}

void AAZGameMode_Server::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}


