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
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/Manager\AZInventoryManager.h"

#include  "Engine/GameInstance.h"
//FIXME merged need del
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/Manager\AZPlayerMgr.h"
//FIXME merged need del
#include <GameFramework/Character.h>

//MinSuhong Add
#include "App_Server.h" // 서버 클래스 [ODBC 연결되어 있음 Packet.h]
#include "Client_To_Server.h" // 서버 접속 클래스
#include "Odbc.h"
#include "TimerManager.h"
#include "UserManager.h"
#include "PlayerController/AZPlayerController_Server.h"

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

	CreateSocketHolder();
	
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAZGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAZGameInstance::EndLoadingScreen);

	if (client_connect == nullptr)
	{
		client_connect = NewObject<UClient_To_Server>(this, TEXT("client_to_server"));
	}

	if (iocp_net_server_ == nullptr)
	{
		iocp_net_server_ = NewObject<UApp_Server>(this, TEXT("iocp_net_server_"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Start Client!"));
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UAZGameInstance::TimerProcessPacket, 0.01f, true);
}

void UAZGameInstance::Shutdown()
{
	Super::Shutdown();

	UE_LOG(LogTemp, Warning, TEXT("Shutdown!"));

	if (iocp_net_server_->server_check_ == true)
	{
		iocp_net_server_->End();

		//delete iocp_net_server_;
	}

	if (client_connect->client_check == true)
	{
		client_connect->Client_Shutdown();
	}

	DestroySocketHolder();
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
	for (uint8 socket_type = 0; socket_type < (int32)ESocketHolderType::Max; ++socket_type)
	{
		UAZSocketHolder* socket_hodler = NewObject<UAZSocketHolder>();
		socket_hodler->Init((ESocketHolderType)socket_type);
		array_socket_holder_.Add(socket_hodler);
	}
}

void UAZGameInstance::DestroySocketHolder()
{
	for (UAZSocketHolder* socket_holder : array_socket_holder_)
	{
		if (socket_holder != nullptr)
		{
			socket_holder->Disconnect();
			socket_holder = nullptr;
		}
	}
}

void UAZGameInstance::InitSocketOnMapLoad()
{
	/*for (ULHSocketHolder* pSocketHolder : ArraySocketHolder)
	{
		if (pSocketHolder)
		{
			pSocketHolder->ArrangeProtocolExchangeEvents();
		}
	}*/
}

UAZSocketHolder* UAZGameInstance::GetSocketHolder(ESocketHolderType socket_type)
{
	if (array_socket_holder_.Num() != (int32)ESocketHolderType::Max)
	{
		return nullptr;
	}

	if (socket_type >= ESocketHolderType::Max)
	{
		return nullptr;
	}

	if (GetGameMode() == nullptr)
	{
		return nullptr;
	}
	return array_socket_holder_[(int32)socket_type];
}

bool UAZGameInstance::IsWaitingProtocolEmpty()
{
	bool is_empty = true;
	for (auto socket_holder : array_socket_holder_)
	{
		if (socket_holder == nullptr)
		{
			continue;
		}

		if (socket_holder->IsWaitingProtocolEmpty() == false)
		{
			is_empty = false;
		}
	}
	return is_empty;
}

TArray<FString> UAZGameInstance::GetWaitingPorotocolNames() const
{
	TArray<FString> result_array;
	for (auto socket_holder : array_socket_holder_)
	{
		result_array.Append(socket_holder->GetWaitingProtocolNames());
	}

	return result_array;
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
	if (controller == nullptr)
	{
		return nullptr;
	}
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

void UAZGameInstance::TimerProcessPacket()
{
	UE_LOG(LogTemp, Warning, TEXT("TimerProcessPacket"));

	if (auto packet_data = DequePacketData(); packet_data.packet_id_ > (UINT16)PACKET_ID::SYS_END)
	{
		//is_idle = false;
		// 요청이 있는 경우 처리
		ProcessRecvPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
	}

	// 시스템 패킷 (연결 & 연결 종료가 발생한 경우)
	if (auto packet_data = DequeSystemPacketData(); packet_data.packet_id_ != 0)
	{
		//is_idle = false;
		// 요청이 있는 경우 처리
		ProcessRecvPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
	}

	if (timer_destroy_sw)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(TimerHandle);
	}
}

void UAZGameInstance::ProcessPacket()
{
	UE_LOG(LogTemp, Warning, TEXT("ProcessPacket"));

	while (is_run_process_thread_)
	{
		bool is_idle = true;

		// 이미 연결이 된 유저가 보낸 요청이 있는 경우
		if (auto packet_data = DequePacketData(); packet_data.packet_id_ > (UINT16)PACKET_ID::SYS_END)
		{
			is_idle = false;
			// 요청이 있는 경우 처리
			ProcessRecvPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
		}

		// 시스템 패킷 (연결 & 연결 종료가 발생한 경우)
		if (auto packet_data = DequeSystemPacketData(); packet_data.packet_id_ != 0)
		{
			is_idle = false;
			// 요청이 있는 경우 처리
			ProcessRecvPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
		}

		if (is_idle)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void UAZGameInstance::PacketInit(const UINT32 max_client)
{
	// unordered_map
	recv_funtion_dictionary_ = std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION>();

	// (PACKET_ID 설정) Key, Value 설정
	recv_funtion_dictionary_[(int)PACKET_ID::SYS_USER_CONNECT] = &UAZGameInstance::ProcessuserConnect;
	recv_funtion_dictionary_[(int)PACKET_ID::SYS_USER_DISCONNECT] = &UAZGameInstance::ProcessUserDisConnect;
	recv_funtion_dictionary_[(int)PACKET_ID::LOGIN_REQUEST] = &UAZGameInstance::ProcessLogin;
	recv_funtion_dictionary_[(int)PACKET_ID::SIGNIN_REQUEST] = &UAZGameInstance::ProcessSignup;
	recv_funtion_dictionary_[(int)PACKET_ID::CHAT_SEND_REQUEST] = &UAZGameInstance::ProcessChatting;
	recv_funtion_dictionary_[(int)PACKET_ID::IN_GAME_REQUEST] = &UAZGameInstance::ProocessInGame;
	//recv_funtion_dictionary_[(int)PACKET_ID::IN_GAME_MOVE_START] = &UAZGameInstance::ProocessInPlayerMove;

	CreateCompent(max_client);
}

void UAZGameInstance::PacketEnd()
{
	is_run_process_thread_ = false;

	if (process_thread_.joinable())
	{
		process_thread_.join();
	}
}

void UAZGameInstance::DbRun()
{
	odbc->Init();
	odbc->ConnectMssql(L"odbc_test.dsn");
	odbc->Load();
}

void UAZGameInstance::ReceivePacketData(const UINT32 client_index, const UINT32 size, char* P_data)
{
	UE_LOG(LogTemp, Warning, TEXT("ReceivePacketData_GameInstance client : %d, size : %d"), client_index, size);
	// GetUserByConnIdx 어떤 클라이언트가 메세지를 보냈는지 확인
	auto P_user = user_manager_->GetUserByConnIdx(client_index);

	// 메세지를 보낸 클라이언트 객체에 데이터를 담음
	P_user->SetPacketData(size, P_data);

	// 큐에 리시브가 발생했다는 것을 알려줌
	EnqueuePacketData(client_index);
}

void UAZGameInstance::PushSystemPacket(PacketInfo packet)
{
	std::lock_guard<std::mutex> guard(lock_);
	// 패킷 헤더에 따라서 처리
	system_packet_queue_.push_back(packet);
}

SQLTCHAR* UAZGameInstance::ConvertCharToSqlTCHAR(const char* charArray)
{
	int charArrayLen = strlen(charArray) + 1;
	int numChars = MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, NULL, 0);

	SQLTCHAR* sqlTCharArray = new SQLTCHAR[numChars];

	MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, (LPWSTR)sqlTCharArray, numChars);

	return sqlTCharArray;
}

void UAZGameInstance::CreateCompent(const UINT32 max_client)
{
	user_manager_ = new UserManager;
	user_manager_->Init(max_client);
	odbc = new Odbc;
}

void UAZGameInstance::ClearConnectionInfo(INT32 client_index)
{
	auto P_req_user = user_manager_->GetUserByConnIdx(client_index);

	if (P_req_user->GetDomainState() != User::DOMAIN_STATE::NONE)
	{
		user_manager_->DeleteUserInfo(P_req_user);
	}
}

void UAZGameInstance::EnqueuePacketData(const UINT32 client_index)
{
	std::lock_guard<std::mutex> guard(lock_);
	server_in_coming_packet_user_index_.push_back(client_index);
}

PacketInfo UAZGameInstance::DequePacketData()
{
	UINT32 user_index = 0;

	{
		std::lock_guard<std::mutex> grard(lock_);
		// 현재 Send 요청을 보낸 유저가 있는지 확인
		if (server_in_coming_packet_user_index_.empty())
		{
			return PacketInfo();
		}

		// 요청한 데이터가 있다면 index 추출
		user_index = server_in_coming_packet_user_index_.front();
		server_in_coming_packet_user_index_.pop_front();
	}

	// 알아낸 index로 유저 객체 가져옴
	auto P_user = user_manager_->GetUserByConnIdx(user_index);
	auto packet_data = P_user->GetPacket();
	packet_data.client_index_ = user_index;

	return packet_data;
}

PacketInfo UAZGameInstance::DequeSystemPacketData()
{
	std::lock_guard<std::mutex> guard(lock_);
	if (system_packet_queue_.empty())
	{
		return PacketInfo();
	}

	auto packet_data = system_packet_queue_.front();
	system_packet_queue_.pop_front();

	return packet_data;
}

void UAZGameInstance::ProcessRecvPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet)
{
	// 요청한 패킷 id로 클라이언트 객체 찾음
	auto iter = recv_funtion_dictionary_.find(packet_id);
	if (iter != recv_funtion_dictionary_.end())
	{
		// 찾은 객체로 처리
		(this->*(iter->second))(client_index, packet_size, P_packet);
	}
}

void UAZGameInstance::ProcessuserConnect(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	// 새로 연결 받기
	printf("[ProcessUserConnect_Gameinstance] clientIndex: %d\n", client_index);
	// 하나의 user_manger 객체 할당
	auto P_user = user_manager_->GetUserByConnIdx(client_index);
	// 객체 초기화
	P_user->Clear();
}

void UAZGameInstance::ProcessUserDisConnect(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	printf("[ProcessUserDisConnect_Gameinstance] clientIndex: %d\n", client_index);
	// 연결이 끊어진 경우 user 반환
	ClearConnectionInfo(client_index);
}

void UAZGameInstance::ProcessLogin(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	auto  P_login_req_packet = reinterpret_cast<LOGIN_REQUEST_PACKET*>(P_packet);

	auto P_user_id = ConvertCharToSqlTCHAR(P_login_req_packet->user_id_);
	auto P_user_pw = ConvertCharToSqlTCHAR(P_login_req_packet->user_pw_);
	UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	if (odbc->LoginCheckSQL(P_user_id, P_user_pw))
	{

		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		Login_Send_Packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::LOGIN_RESPONSE_SUCCESS;
		login_res_packet.packet_length = sizeof(login_res_packet);
		//login_res_packet.clinet_id = client_index;

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		Login_Send_Packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::LOGIN_RESPONSE_FAIL;
		login_res_packet.packet_length = sizeof(login_res_packet);

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
}

void UAZGameInstance::ProcessSignup(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	dbitem record;

	auto  P_signup_req_packet = reinterpret_cast<LOGIN_REQUEST_PACKET*>(P_packet);

	auto P_user_id = ConvertCharToSqlTCHAR(P_signup_req_packet->user_id_); ;
	auto P_user_pw = ConvertCharToSqlTCHAR(P_signup_req_packet->user_pw_); ;

	UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	record.name = P_user_id;
	record.pass = P_user_pw;

	if (Fclient_connect_.IsBound() == true)	Fclient_connect_.Execute();

	if (odbc->AddSQL(record))
	{
		//odbc.Load();

		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		Login_Send_Packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::SIGNIN_RESPONSE_SUCCESS;
		login_res_packet.packet_length = sizeof(login_res_packet);
		//login_res_packet.clinet_id = client_index;

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		Login_Send_Packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::SIGNIN_RESPONSE_FAIL;
		login_res_packet.packet_length = sizeof(login_res_packet);
		//login_res_packet.clinet_id = client_index;

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
}

void UAZGameInstance::ProcessChatting(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	Defind defind;
	auto  P_login_req_packet = reinterpret_cast<Login_Send_Packet*>(P_packet);

	Login_Send_Packet login_res_packet;
	login_res_packet.packet_id = (int)PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS;
	login_res_packet.packet_length = sizeof(login_res_packet);
	//login_res_packet.clinet_id = client_index;
	strcpy_s(login_res_packet.user_id, sizeof(login_res_packet.user_id), P_login_req_packet->user_id);

	UE_LOG(LogTemp, Warning, TEXT("[ProcessChatting Send_GameInstance] packet_id : %d, packet_length : %d packet_data %s\n"),
		login_res_packet.packet_id, login_res_packet.packet_length, *defind.CharArrayToFString(login_res_packet.user_id));

	BroadCastSendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
}

void UAZGameInstance::ProocessInGame(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	SetMoveInfo move_info_packet;
	move_info_packet.packet_id = (int)PACKET_ID::IN_GAME_SUCCESS;
	move_info_packet.packet_length = sizeof(move_info_packet);
	//move_info_packet.clinet_id = client_index;
	move_info_packet.fvector_ = FVector(100.0f, 0.0f, 0.0f);
	move_info_packet.frotator_ = FRotator(0.0f, 0.0f, 500.0f);

	Cast<AAZPlayerController_Server>(GetPlayerController())->AddPlayer_Origin(client_index);
	//이미 서버에 있는 유저들에게도 Remotable_AddPlayer
	
	UE_LOG(LogTemp, Warning, TEXT("[ProocessInGame_GameInstance] fvector_ : %s / frotator_ : %s\n"), *move_info_packet.fvector_.ToString(), *move_info_packet.frotator_.ToString());

	BroadCastSendPacketFunc(client_index, sizeof(move_info_packet), (char*)&move_info_packet);
}

void UAZGameInstance::ProocessInPlayerMove(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	auto  P_move_info_packet = reinterpret_cast<SetMoveInfo*>(P_packet);
	SetMoveInfo move_info_packet;
	//move_info_packet.packet_id = (int)PACKET_ID::IN_GAME_MOVE_END;
	move_info_packet.packet_length = sizeof(move_info_packet);
	//move_info_packet.clinet_id = client_index;
	move_info_packet.fvector_ = P_move_info_packet->fvector_;
	move_info_packet.frotator_ = P_move_info_packet->frotator_;

	UE_LOG(LogTemp, Warning, TEXT("[move_info_2_GameInstance] client : %d / fvector_ : %s / frotator_ : %s\n"), client_index, *move_info_packet.fvector_.ToString(), *move_info_packet.frotator_.ToString());

	BroadCastSendPacketFunc(client_index, sizeof(move_info_packet), (char*)&move_info_packet);
}
