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
#include "Character/Monster/AZMonster.h"
#include "Character/Player/AZPlayer_Origin.h"
#include "CharacterComponent/AZMonsterHealthComponent.h"
#include "Engine/LevelStreamingDynamic.h"
#include "PlayerState/AZPlayerState_Client.h"


AAZGameMode_Server::AAZGameMode_Server()
{
	PrimaryActorTick.bCanEverTick = true;
	//bNetLoadOnClient = false;
	//bPauseable = true;
	//bStartPlayersAsSpectators = false;

	DefaultPawnClass = ADefaultPawn::StaticClass();
	PlayerControllerClass = AAZPlayerController_Server::StaticClass();
	
	//Player처리하기
	//연결된 클라이언트의 플레이어 컨트롤러로 부터 입력값을 전송받는다.
	//입력값을 전송받은 후 그것을 동일한 플레이어의 클론 캐릭터에 전달한다.(결국 플레이어블을 4개 가지고 플레이하는것과 같다.)
	
	PlayerStateClass = AAZPlayerState_Server::StaticClass();//Server State
	GameStateClass = AAZGameState_Server::StaticClass();//Server State
	// showdebug 못해서 임시로 막아둠
	//HUDClass = AHUD::StaticClass();//Server UI

	//GameSessionClass = AGameSession::StaticClass();
	//SpectatorClass = ASpectatorPawn::StaticClass();
	//ReplaySpectatorPlayerControllerClass = APlayerController::StaticClass();
	//ServerStatReplicatorClass = AServerStatReplicator::StaticClass();
}

// Called when the game starts or when spawned
void AAZGameMode_Server::BeginPlay()
{
	Super::BeginPlay();

	// TEMP 서버에서는 스트리밍 레벨 다 로드해두기
	bool is_succeeded = false;
	// 패키징할때 설명 확인 MUST
	combat_level_ = ULevelStreamingDynamic::LoadLevelInstance(this, TEXT("StreamingMap_Combat"),
		FVector::ZeroVector, FRotator::ZeroRotator, is_succeeded);
	combat_level_->SetShouldBeLoaded(true);
	combat_level_->SetShouldBeVisible(true);
}

void AAZGameMode_Server::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	Super::InitGame(map_name, options, error_message);
	object_mgr_ = NewObject<UAZObjectMgr_Server>(this);
}

void AAZGameMode_Server::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	// Debug log
	if (GEngine && !object_mgr_->object_map_.IsEmpty())
	{
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::White, FString::Printf(TEXT("---------------------")));
		for (const auto monster : object_mgr_->spawned_monsters_array_)
		{
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Green,
				FString::Printf(TEXT("  Monster[%d] HP: %d"), monster->object_serial_, monster->health_component_->current_hp_));
		}
		for (const auto player_pair : object_mgr_->player_map_)
		{
			GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::Purple,
				FString::Printf(TEXT("  Player[%d] HP: %d"),
					player_pair.Key, player_pair.Value->player_character_state_->character_state_.current_health_point));
		}
		GEngine->AddOnScreenDebugMessage(-1, GetWorld()->GetDeltaSeconds(), FColor::White, FString::Printf(TEXT("---------------------")));
	}
}
