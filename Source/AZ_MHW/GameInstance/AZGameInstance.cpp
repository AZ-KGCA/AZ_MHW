// Copyright Team AZ. All Rights Reserved.

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
#include "AZ_MHW/GameMode/AZGameMode_InGame.h"
#include "AZ_MHW/HUD/AZHUD_InGame.h"
#include "..\Manager\AZInventoryManager.h"
#include  "Engine/GameInstance.h"
//FIXME merged need del
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "..\Manager\AZPlayerAssetMgr.h"
//FIXME merged need del
#include <GameFramework/Character.h>

//MinSuhong Add
#include "AppServer.h" // 서버 클래스 [ODBC 연결되어 있음 Packet.h]
#include "Client_To_Server.h" // 서버 접속 클래스

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
	Super::Init();

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

	inventory_mgr = NewObject<UAZInventoryManager>();
	inventory_mgr->Init();
	
	input_mgr_ = NewObject<UAZInputMgr>();
	input_mgr_->Init();

	AddNewSingleton(map_mgr = NewObject<UAZMapMgr>(this));
	msg_handler->OnRegister(map_mgr);
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAZGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAZGameInstance::EndLoadingScreen);

	//if (client_connect == nullptr)
	//{
	//	client_connect = NewObject<UClient_To_Server>(this, TEXT("client_to_server"));
	//}

	//iocp_net_server_ = new AppServer;

	UE_LOG(LogTemp, Warning, TEXT("Start Client!"));
}

void UAZGameInstance::Shutdown()
{
	Super::Shutdown();

	UE_LOG(LogTemp, Warning, TEXT("Shutdown!"));

	//if (iocp_net_server_->server_check_ == true)
	//{
	//	iocp_net_server_->End();

	//	delete iocp_net_server_;
	//}

	//if (client_connect->client_check == true)
	//{
	//	client_connect->Client_Shutdown();
	//}

	//SendLogoutCmd();

	mgrs.Reset();
	AZ_LOG("Shutdown");

	// Destroy in reverse order of creation
	game_option = nullptr;
	login_mgr = nullptr;
	save_data = nullptr;
	game_config = nullptr;
	map_mgr = nullptr;
	hud_data_mgr = nullptr;
	input_mgr_ = nullptr;
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

void UAZGameInstance::CreateSocketHolder()
{
	// FIXME merged socket create
}

void UAZGameInstance::DestroySocketHolder()
{
	// FIXME merged socket del
}

void UAZGameInstance::InitSocketOnMapLoad()
{
	// FIXME merged socket init
}

AAZHUD* UAZGameInstance::GetHUD()
{
	if (GetPlayerController() == nullptr)
	{
		return nullptr;
	}
	auto hud = Cast<AAZHUD>(GetPlayerController()->GetHUD());
	if (hud == nullptr)
	{
		return nullptr;
	}
	return hud;
}

AAZHUD_InGame* UAZGameInstance::GetHUDInGame()
{
	return GetPlayerController() ? Cast<AAZHUD_InGame>(GetPlayerController()->GetHUD()) : nullptr;
}

AAZGameMode* UAZGameInstance::GetGameMode()
{
	if (GetWorld() == nullptr)
	{
		return nullptr;
	}
	return GetWorld() ? Cast<AAZGameMode>(GetWorld()->GetAuthGameMode()) : nullptr;
}

AAZGameMode_InGame* UAZGameInstance::GetInGameMode()
{
	return Cast<AAZGameMode_InGame>(GetGameMode());
}

UAZGameState* UAZGameInstance::GetCurGameState()
{
	auto* game_mode = GetGameMode();
	if (game_mode == nullptr)
	{
		return nullptr;
	}
	return game_mode->GetCurGameState();
}

AAZPlayerController* UAZGameInstance::GetPlayerController()
{
	if (GetWorld() == nullptr)
	{
		return nullptr;
	}

	auto controller = Cast<AAZPlayerController>(GetFirstLocalPlayerController(GetWorld()));
	return controller;
}

AAZPlayer_Playable* UAZGameInstance::GetPlayer()
{
	auto* player_controller = GetPlayerController();
	if (player_controller == nullptr)
	{
		return nullptr;
	}
	//FIXME working check
	auto player = player_controller->GetPawn<AAZPlayer_Playable>();
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