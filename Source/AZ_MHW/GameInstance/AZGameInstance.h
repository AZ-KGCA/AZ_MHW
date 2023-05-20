
// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Packet.h"
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <memory>
#include "Engine/GameInstance.h"
#include "AZ_MHW/GameInstance/AZGameInstanceData.h"
#include "AZ_MHW/PlayerController/AZPlayerController.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZGameInstance.generated.h"

/**
 * 
 */

DECLARE_DELEGATE(FClient_Connect); // 접속 델리게이트

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

///////////////////
// 패킷처리 추가 //
///////////////////
public:
	bool server_client_check = false;
	bool timer_destroy_sw = false;

public:
	FTimerHandle TimerHandle;

	void TimerProcessPacket();

	void ProcessPacket();

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

private:
	void CreateCompent(const UINT32 max_client);

	void ClearConnectionInfo(INT32 client_index);

	void EnqueuePacketData(const UINT32 client_index);

	PacketInfo DequePacketData();

	PacketInfo DequeSystemPacketData();

	void ProcessRecvPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet);

	void ProcessuserConnect(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessUserDisConnect(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessLogin(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessSignup(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessChatting(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProocessInGame(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProocessInPlayerMove(UINT32 client_index, UINT16 packet_size, char* P_packet);

	typedef void (UAZGameInstance::* PROCESS_RECV_PACKET_FUNCTION)(UINT32, UINT16, char*);

	std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION> recv_funtion_dictionary_;

	UserManager* user_manager_;

	Odbc* odbc;

	std::function<void(int, char*)> send_mq_data_func_;

	std::thread process_thread_;

	std::mutex lock_;

	// 실제 데이터가 왔을때 사용 하는 큐
	std::deque<UINT32> server_in_coming_packet_user_index_;

	// 네트워크 연결 & 끊어짐을 처리하는 큐
	std::deque<PacketInfo> system_packet_queue_;

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
	UPROPERTY() class UAZInputMgr* input_mgr_;
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