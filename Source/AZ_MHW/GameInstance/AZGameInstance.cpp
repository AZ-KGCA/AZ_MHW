// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include <Kismet/GameplayStatics.h>
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/ClientMsg/AZMsgHandler.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZ_MHW/Manager/AZGameConfig.h"
#include "AZ_MHW/Manager/AZGameOption.h"
#include "AZ_MHW/Manager/AZSaveData.h"
#include "AZ_MHW/Manager/SaveData/GameOptionSaveData.h"
#include "AZ_MHW/HUD/AZHUDDataMgr.h"
#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/Login/AZLoginMgr.h"

//FIXME 병합시 삭제
#include <GameFramework/Character.h>

UGameInstanceProxy AZGameInstance;

UAZGameInstance::UAZGameInstance()
{
	AZGameInstance = this;
}
UAZGameInstance::~UAZGameInstance()
{
	AZGameInstance = nullptr;
}

void UAZGameInstance::Init()
{
	UAZGameSingleton::instance();

	msg_handler = NewObject<UAZMsgHandler>(this);

	game_config = NewObject<UAZGameConfig>();
	game_config->Load();

	save_data = NewObject<UAZSaveData>();
	save_data->LoadAll();

	hud_data_mgr = NewObject<UAZHUDDataMgr>();
	hud_data_mgr->Init();

	game_option = NewObject<UAZGameOption>();
	game_option->Init();
	UAZGameOption::option_save_data = &save_data->option_save_data->game_option_save_data;

	login_mgr = NewObject<UAZLoginMgr>();
	login_mgr->Init();

	AddNewSingleton(map_mgr = NewObject<UAZMapMgr>(this));
	msg_handler->OnRegister(map_mgr);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAZGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAZGameInstance::EndLoadingScreen);
}

void UAZGameInstance::Shutdown()
{
	SendLogoutCmd();

	mgrs.Reset();
	AZ_LOG("Shutdown");

	// 생성과 역순으로 소멸
	game_option = nullptr;
	login_mgr = nullptr;
	save_data = nullptr;
	game_config = nullptr;
	map_mgr = nullptr;
	hud_data_mgr = nullptr;
}

void UAZGameInstance::RestMgr()
{
	for (auto* mgr : mgrs)
	{
		mgr->ResetMgr();
	}
	AZ_LOG("ResetMgr");
}

void UAZGameInstance::LobbyEnterAck()
{
	for (auto* mgr : mgrs)
	{
		mgr->LobbyEnterAck();
	}
	AZ_LOG("LobbyEnterAck");
}

void UAZGameInstance::PlayerSelectAck()
{
	for (auto* mgr : mgrs)
	{
		mgr->PlayerSelectAck();
	}
	AZ_LOG("PlayerSelectAck");
}

AAZHUD* UAZGameInstance::GetHUD()
{
	// FIXME
	return nullptr;
}

// FIXME
/*AAZHUD_InGame* UAZGameInstance::GetHUDInGame()
{
	// FIXME
	return nullptr;
}*/

AAZGameMode* UAZGameInstance::GetGameMode()
{
	if (GetWorld() == nullptr)
	{
		return nullptr;
	}
	return GetWorld() ? Cast<AAZGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
}

//FIXME
/*AAZGameMode_InGame* UAZGameInstance::GetInGameMode()
{
	// FIXME
	return nullptr;
}*/

UAZGameState* UAZGameInstance::GetCurGameState()
{
	auto* game_mode = GetGameMode();
	if (game_mode == nullptr)
	{
		return nullptr;
	}
	return game_mode->GetCurGameState();
}

APlayerController* UAZGameInstance::GetPlayerController()
{
	if (GetWorld() == nullptr)
	{
		return nullptr;
	}

	auto* controller = Cast<APlayerController>(GetFirstLocalPlayerController(GetWorld()));
	return controller;
}

ACharacter* UAZGameInstance::GetPlayer()
{
	auto* player_controller = GetPlayerController();
	if (player_controller == nullptr)
	{
		return nullptr;
	}
	// FIXME 병합시 고치기
	auto* player = player_controller->GetPawn<ACharacter>();
	return player;
}

void UAZGameInstance::BeginLoadingScreen(const FString& map_name)
{
}

void UAZGameInstance::EndLoadingScreen(UWorld* in_loaded_world)
{
}

void UAZGameInstance::OnGameMsg(FAZGameMsg* game_msg)
{
}

void UAZGameInstance::SendLogoutCmd()
{
}

void UAZGameInstance::AddNewSingleton(UAZSingletonObject* mgr)
{
}
