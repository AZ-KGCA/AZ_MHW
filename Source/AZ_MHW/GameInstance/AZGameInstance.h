// Copyright Team AZ. All Rights Reserved.

#pragma once

//MinSuhong
// IOCP
#include "IocpNetServer.h"
#include <string>
#include <iostream>
#include "AppServer.h" // ODBC ���� Packet.h

#include "Windows.h"

// window �⺻ Ÿ�� Hide
#include "Windows/AllowWindowsPlatformTypes.h"
#include "Windows/prewindowsapi.h"
#include "Windows/PostWindowsApi.h"
#include "Windows/HideWindowsPlatformTypes.h"
#pragma comment(lib, "ws2_32.lib")
#include <WinSock2.h>

// ODBC ����
#include "Odbc.h"

// ���� ���� Ŭ����
#include "Client_To_Server.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AZ_MHW/GameInstance/AZGameInstanceData.h"
#include "AZGameInstance.generated.h"

const UINT16 SERVER_PORT = 10000;
const UINT16 MAX_CLIENT = 5;		//�� �����Ҽ� �ִ� Ŭ���̾�Ʈ ��
const int SLEEP_TIME = 3000;
const UINT32 MAX_IO_WORKER_THREAD = 4;

//MinSuhong End

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameInstance : public UGameInstance
{
	GENERATED_BODY()
	//MinSuhong
public:
	FSocket* fsocket_version;
	SOCKET win_socket;

	WSADATA wsa;
	Odbc g_odbc;

	AppServer iocp_net_server_;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Server_Chioce)
		int32 socket_type = 10;

	//Client_Connect client_connect;

public:
	//UPROPERTY(Transient)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UClient_To_Server* client_connect;
public: // ��ǥ�� �޴� ��
	//FVector CurrentLocation;
	TArray<FVector> current_location_array;
	int32 index_num = 0;

public:
	void PacketToServer(const char* packet, int packet_size);

public:
	bool LoginRecord(FString login_id, FString login_pw);
	bool SignupRecord(FString signup_id, FString signup_pw);
	void IocpServerStart();

	bool ClientSignin();

	// socket_type = 0
	UFUNCTION(BlueprintCallable, Category = Game_Instance)
		void FSocketConncet();

	// socket_type = 1
	UFUNCTION(BlueprintCallable, Category = Game_Instance)
		void WinSocketConnect();

	// socket_type = 2
	UFUNCTION(BlueprintCallable, Category = Game_Instance)
		void WinSocketOpen();

	// socket_type = 3
	UFUNCTION(BlueprintCallable, Category = Game_Instance)
		void IocpServerOpen();

	// socket_type = 4
	UFUNCTION(BlueprintCallable, Category = Game_Instance)
		void OdbcConnect();

	// socket_type = 5
	UFUNCTION(BlueprintCallable, Category = Game_Instance)
		void IocpOdbcOpen();

	//  socket_type = 6
	UFUNCTION(BlueprintCallable, Category = Game_Instance)
		void ServerConnect();
	//MinSuhong End
public:
	// test
	UPROPERTY() class UAZGameConfig* game_config;
	UPROPERTY() class UAZSaveData* save_data;
	UPROPERTY() class UAZMsgHandler* msg_handler;
	UPROPERTY() class UAZLoginMgr* login_mgr;
	UPROPERTY() class UAZMapMgr* map_mgr;
	UPROPERTY() class UAZHUDDataMgr* hud_data_mgr;
	UPROPERTY() class UAZGameOption* game_option;
	UPROPERTY() class UAZInventoryManager* inventory_mgr;
	UPROPERTY() class UAZInputMgr* input_mgr;
	UPROPERTY() class UAZPlayerAssetMgr* playerAsset_mgr;
	UPROPERTY() FMapChannelInfo map_channel_info;
	
public:
	UAZGameInstance();
	virtual ~UAZGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	void RestMgr();
	void LobbyEnterAck();
	void PlayerSelectAck();

	void CreateSocketHolder();
	void DestroySocketHolder();
	void InitSocketOnMapLoad();

	UFUNCTION(BlueprintCallable, Category = "AZ") 
	class AAZHUD* GetHUD();
	//UFUNCTION(BlueprintCallable, Category = "AZ")
	//class AAZHUD_InGame* GetHUDInGame();

	UFUNCTION(BlueprintCallable, Category = "AZ")
	class AAZGameMode* GetGameMode();

	//UFUNCTION(BlueprintCallable, Category = "AZ")
	//class AAZGameMode_InGame* GetInGameMode();

	class UAZGameState* GetCurGameState();

	// FIXME merged need edit
	UFUNCTION(BlueprintCallable, Category = "AZ")
	APlayerController* GetPlayerController();
	// FIXME merged need edit
	ACharacter* GetPlayer();

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& map_name);

	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* in_loaded_world);

public:
	void OnGameMsg(struct FAZGameMsg* game_msg);

private:
	void SendLogoutCmd();
	void AddNewSingleton(class UAZSingletonObject* mgr);

private:
	UPROPERTY() TArray<class UAZSingletonObject*> mgrs;
};

class UGameInstanceProxy
{
public:
	UGameInstanceProxy() :
		game_instance_(nullptr)
	{};

	inline UAZGameInstance* operator->()
	{
		// GInst is changed often on the game thread when in PIE, accessing on any other thread is going to be a race condition
		// In general, the rendering thread should not dereference UObjects, unless there is a mechanism in place to make it safe
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

	inline const UAZGameInstance* operator->() const
	{
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

	inline UAZGameInstance& operator*()
	{
		//checkSlow(IsInGameThread());
		return *game_instance_;
	}

	inline const UAZGameInstance& operator*() const
	{
		//checkSlow(IsInGameThread());
		return *game_instance_;
	}

	inline UGameInstanceProxy& operator=(UAZGameInstance* InWorld)
	{
		game_instance_ = InWorld;
		return *this;
	}

	inline UGameInstanceProxy& operator=(const UGameInstanceProxy& InProxy)
	{
		game_instance_ = InProxy.game_instance_;
		return *this;
	}

	inline bool operator==(const UGameInstanceProxy& Other) const
	{
		return game_instance_ == Other.game_instance_;
	}

	inline operator UAZGameInstance* () const
	{
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

	inline UAZGameInstance* GetReference()
	{
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

private:
	UAZGameInstance* game_instance_;
};

extern AZ_MHW_API class UGameInstanceProxy AZGameInstance;
