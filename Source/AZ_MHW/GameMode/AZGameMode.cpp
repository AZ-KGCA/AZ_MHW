// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/GameMode/AZGameMode.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameStateMachine/AZStateMachine.h"
#include "AZ_MHW/GameStateMachine/AZGameState.h"
#include "AZ_MHW/HUD/AZHUD.h"

AAZGameMode::AAZGameMode()
{
	// FIXME (병합 시 추가)
	//DefaultPawnClass = ALHPlayer_Playable::StaticClass();
	// FIXME (병합 시 추가)
	//PlayerControllerClass = ALHPlayerController::StaticClass();
	HUDClass = AAZHUD::StaticClass();

	PrimaryActorTick.bCanEverTick = true;

	game_mode_flag = EGameModeFlag::None;

	is_started = false;
}

AAZGameMode::~AAZGameMode()
{

}

void AAZGameMode::InitGame(const FString& map_name, const FString& options, FString& error_message)
{
	//Super::InitGame
	Super::InitGame(map_name, options, error_message);

	//if ()
}

void AAZGameMode::InitGameState()
{
	Super::InitGameState();
}

void AAZGameMode::StartPlay()
{
	// FixME
	/*if (AZGameInstance->map_mgr == nullptr ||
		AZGameInstance->map_mgr->GetMapData() == nullptr)
	{
		StartPlayManually();
	}*/
}

void AAZGameMode::GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList)
{
	Super::GetSeamlessTravelActorList(bToTransition, ActorList);

	// FixMe
	/*TArray<AActor*> actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALHActor_LoadingScene::StaticClass(), actors);
	for (auto actor : actors)
	{
		ActorList.Add(actor);
	}*/
}

void AAZGameMode::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	//FixMe(서버 클라를 게임모드만 갈아 끼우면 될듯(결국 맵으로 분리될듯?)
	//if (!bIsDisconnectSocketHandler)
	//	_RecvPacket();

	//if (StateMachine)
	//{
	//	StateMachine->Tick(deltaSeconds);
	//}

	//if (GetWorld())
	//	PreRealTime = GetWorld()->GetRealTimeSeconds();
}

void AAZGameMode::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);
}

void AAZGameMode::OnGameMsg(FAZGameMsg* new_msg)
{
	//if (StateMachine)
	//{
	//	StateMachine->OnGameMsg(newMsg);
	//}
}

void AAZGameMode::BeginPlay()
{
	Super::BeginPlay();

	//PlatformSetting();

	//PostLightLoaded();
}

UAZStateMachine* AAZGameMode::GetStateMachine() const
{
	return state_machine;
}

void AAZGameMode::SetGameState(EGameState state_id)
{
	check(state_machine);
	state_machine->ChangeState(state_id);
}

void AAZGameMode::SetPrevGameState()
{
	check(state_machine);
	state_machine->SetPreState();
}

UAZGameState* AAZGameMode::GetCurGameState() const
{
	if (state_machine == nullptr)
	{
		return nullptr;
	}

	return state_machine->GetCurState<UAZGameState>();
}

UAZGameState* AAZGameMode::GetGameState(EGameState state_id) const
{
	if (state_machine == nullptr)
	{
		return nullptr;
	}

	return state_machine->GetState(state_id);
}

EGameState AAZGameMode::GetCurGameStateID() const
{
	return GetStateMachine() ? GetStateMachine()->GetCurStateID() : EGameState::None;
}
