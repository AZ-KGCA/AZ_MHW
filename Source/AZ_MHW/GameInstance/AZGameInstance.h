
// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Packet.h"
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <memory>

// client
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <queue>
#pragma comment(lib, "ws2_32.lib")
#include "InGamePacket.h"
// client end

#include "Engine/GameInstance.h"
#include "AZ_MHW/GameInstance/AZGameInstanceData.h"
#include "AZ_MHW/PlayerController/AZPlayerController.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZGameInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FClient_Connect); // 접속 델리게이트

// client

DECLARE_DELEGATE_OneParam(FDle_InGameConnect, const uint16&);

DECLARE_DELEGATE_OneParam(FDle_InGameInit, const FSetMoveInfo&);
DECLARE_DELEGATE_OneParam(FDle_MoveInfo, const FSetMoveInfo&); // 캐릭터 오브젝트 매니저용 델리게이트

//Dynamic
DECLARE_DELEGATE_OneParam(FChat_Broadcast_Success, const FString&); // 채팅 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDele_Dynamic_OneParam, FString, SomeParameter);

DECLARE_DELEGATE_RetVal_TwoParams(bool, RECV_PACKET, UINT32, PACKET_HEADER*);


class UserManager;
class Odbc;

UCLASS()
class AZ_MHW_API UAZGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	//MinSuhong
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UApp_Server* iocp_net_server_;
	 
	//UPROPERTY(Transient)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UClient_To_Server* client_connect;

////////////
// Server //
////////////

public:
	bool server_client_check = false;
	bool timer_destroy_sw = false;

public:
	FTimerHandle server_timer_handle_;
	FTimerHandle client_timer_handle_;

	void TimerProcessPacket();

	bool is_run_process_thread_ = false;

public:
	FClient_Connect Fclient_connect_;

public:

	void PacketInit(const UINT32 max_client);

	void PacketEnd();

	void DbRun();

	void ReceivePacketData(const UINT32 client_index, const UINT32 size, char* P_data);

	void PushSystemPacket(PacketInfo packet);

	std::function<void(UINT32, UINT32, char*)> SendPacketFunc;
	std::function<void(UINT32, UINT32, char*)> BroadCastSendPacketFunc;

	SQLTCHAR* ConvertCharToSqlTCHAR(const char* charArray);
	Odbc* odbc;

private:
	void CreateCompent(const UINT32 max_client);

	void ClearConnectionInfo(INT32 client_index);

	void EnqueuePacketData(const UINT32 client_index);

	PacketInfo DequePacketData();

	PacketInfo DequeSystemPacketData();

	void ProcessSystemPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet);

	void ProcessRecvPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet);

	void ProcessuserConnect(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessUserDisConnect(UINT32 client_index, UINT16 packet_size, char* P_packet);

	UserManager* user_manager_;

	std::function<void(int, char*)> send_mq_data_func_;

	std::thread process_thread_;

	std::mutex lock_;

	// 실제 데이터가 왔을때 사용 하는 큐
	std::deque<UINT32> server_in_coming_packet_user_index_;

	// 네트워크 연결 & 끊어짐을 처리하는 큐
	std::deque<PacketInfo> system_packet_queue_;

	//Server End

// client
public:	// Delegate
	//FDle_InGameConnect Fuc_in_game_connect;
	//FDle_InGameInit Fuc_in_game_init;
	//FDle_MoveInfo Fun_move_info_;
	//FChat_Broadcast_Success Fuc_boradcast_success;

	//UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable, Category = "Event")
	//FDele_Dynamic_OneParam Fuc_Dynamic_OneParam;

	//bool in_game_ = false;
	int32 client_index_;
	RECV_PACKET call_recv_packet_;

public:
	bool Server_Connect(const FString& ip, int32 port);

	void Server_Connect();

	void Client_Shutdown();

	int Server_Packet_Send(const char* packet, int packet_size);

	void receive_thread();

	void ClientTimerProcessPacket();

	// 캐릭터 동기화
public:
	UFUNCTION(BlueprintCallable, Category = Login)
	void InGameAccept();

public:
	SOCKET sock;
	SOCKADDR_IN sa; // 목적지+포트
	LOGIN_REQUEST_PACKET signin_packet;

	std::thread recv_thread_;

	bool recevie_connected = true;

	// Define a queue to store the received data
	std::queue<PACKET_HEADER*> receive_data_queue_;

	// Define a mutex to ensure thread-safe access to the queue
	std::mutex received_data_mutex;

	Defind defind;

	bool client_check = false;

	// 캐릭터 동기화
	FSetMoveInfo set_move_info_;
// client end

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
	UPROPERTY() class UAZInputMgr* input_mgr_;
	UPROPERTY() class UPacketFunction* packet_function_;
	UPROPERTY() FMapChannelInfo map_channel_info;

private:
	UPROPERTY() TArray<class UAZSocketHolder*> array_socket_holder_;
	
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
	
	class UAZSocketHolder* GetSocketHolder(ESocketHolderType socket_type);
	bool IsWaitingProtocolEmpty();
	TArray<FString> GetWaitingPorotocolNames() const;


	UFUNCTION(BlueprintCallable, Category = "AZ") 
	class AAZHUD* GetHUD();
	UFUNCTION(BlueprintCallable, Category = "AZ")
	class AAZHUD_InGame* GetHUDInGame();

	UFUNCTION(BlueprintCallable, Category = "AZ")
	class AAZGameMode* GetGameMode();

	UFUNCTION(BlueprintCallable, Category = "AZ")
	class AAZGameMode_InGame* GetInGameMode();

	class UAZGameState* GetCurGameState();

	// FIXME merged need edit(O)
	UFUNCTION(BlueprintCallable, Category = "AZ")
	AAZPlayerController* GetPlayerController();
	// FIXME merged need edit(O)
	AAZPlayer_Playable* GetPlayer();

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