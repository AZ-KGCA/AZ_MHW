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
#include "App_Server.h" // 서버 클래스 [ODBC 연결되어 있음 Packet.h]
//#include "Client_To_Server.h" // 서버 접속 클래스
#include "Odbc.h"
#include "TimerManager.h"
#include "UserManager.h"

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

	float TimerRate = 1.0f / 30.0f;  // 초당 30회

	if (iocp_net_server_ == nullptr)
	{
		iocp_net_server_ = NewObject<UApp_Server>(this, TEXT("iocp_net_server_"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Start Client!"));
	// 서버 패킷 큐 타이머
	GetWorld()->GetTimerManager().SetTimer(server_timer_handle_, this, &UAZGameInstance::TimerProcessPacket, TimerRate, true);

	// 클라이언트 패킷 큐 타이머
	GetWorld()->GetTimerManager().SetTimer(client_timer_handle_, this, &UAZGameInstance::ClientTimerProcessPacket, TimerRate, true);
}

void UAZGameInstance::Shutdown()
{
	Super::Shutdown();

	UE_LOG(LogTemp, Warning, TEXT("Shutdown!"));

	if (iocp_net_server_->server_check_ == true)
	{
		iocp_net_server_->End();
	}

	if (timer_destroy_sw)
	{
		Client_Shutdown();
	}

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
		timerManager.ClearTimer(server_timer_handle_);
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
	recv_funtion_dictionary_[(int)PACKET_ID::IN_GAME_MOVE_START] = &UAZGameInstance::ProocessInPlayerMove;
	recv_funtion_dictionary_[(int)PACKET_ID::MONSTER_RESPONSE] = &UAZGameInstance::ProocessInPlayerMove;

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
	odbc->ConnetMssql(L"odbc_test.dsn");
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
		login_res_packet.clinet_id = client_index;

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
		login_res_packet.clinet_id = client_index;

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		Login_Send_Packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::SIGNIN_RESPONSE_FAIL;
		login_res_packet.packet_length = sizeof(login_res_packet);
		login_res_packet.clinet_id = client_index;

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
}

void UAZGameInstance::ProcessChatting(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	//Defind defind;
	auto  P_login_req_packet = reinterpret_cast<Login_Send_Packet*>(P_packet);

	Login_Send_Packet login_res_packet;
	login_res_packet.packet_id = (int)PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS;
	login_res_packet.packet_length = sizeof(login_res_packet);
	login_res_packet.clinet_id = client_index;
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
	move_info_packet.clinet_id = client_index;
	move_info_packet.fvector_ = FVector(100.0f, 0.0f, 0.0f);
	move_info_packet.frotator_ = FRotator(0.0f, 0.0f, 500.0f);

	UE_LOG(LogTemp, Warning, TEXT("[ProocessInGame_GameInstance] fvector_ : %s / frotator_ : %s\n"), *move_info_packet.fvector_.ToString(), *move_info_packet.frotator_.ToString());

	BroadCastSendPacketFunc(client_index, sizeof(move_info_packet), (char*)&move_info_packet);
}

void UAZGameInstance::ProocessInPlayerMove(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	auto  P_move_info_packet = reinterpret_cast<SetMoveInfo*>(P_packet);
	SetMoveInfo move_info_packet;
	move_info_packet.packet_id = (int)PACKET_ID::IN_GAME_MOVE_END;
	move_info_packet.packet_length = sizeof(move_info_packet);
	move_info_packet.clinet_id = client_index;
	move_info_packet.fvector_ = P_move_info_packet->fvector_;
	move_info_packet.frotator_ = P_move_info_packet->frotator_;

	UE_LOG(LogTemp, Warning, TEXT("[move_info_2_GameInstance] client : %d / fvector_ : %s / frotator_ : %s\n"), client_index, *move_info_packet.fvector_.ToString(), *move_info_packet.frotator_.ToString());

	BroadCastSendPacketFunc(client_index, sizeof(move_info_packet), (char*)&move_info_packet);
}

// add code
void UAZGameInstance::ProocessMonsterCreate(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	// 클라에서 받은 패킷 풀기
	auto client_monster_packet_ = reinterpret_cast<SetMoveInfo*>(P_packet);

	SetMoveInfo monster_response_packet; // 서버에서 클라로 보낼 패킷 생성
	monster_response_packet.packet_id = (int)PACKET_ID::MONSTER_RESPONSE; // 서버 -> 클라 (602 packet header 지정)
	monster_response_packet.packet_length = sizeof(monster_response_packet); // 보낼 패킷 사이즈
	monster_response_packet.fvector_ = client_monster_packet_->fvector_ + FVector(100.0f, 0.0f, 0.0f); // 클라에서 x 100을 받은거에서 100을 더해서 포장
	monster_response_packet.frotator_ = client_monster_packet_->frotator_; // 클라에서 받은 Rotator 값 그대로 전송

	BroadCastSendPacketFunc(client_index, sizeof(monster_response_packet), (char*)&monster_response_packet); // 현재 서버에 접속한 클라이언트들에게만 전송
}


// client 
void UAZGameInstance::Server_Connect()
{
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	/*----------------------
	SOCKET 생성
	-----------------------*/
	sock = socket(AF_INET, SOCK_STREAM, 0);

	short sData = 10000;
	short tData = 0x2710;
	short fData = 0x1027;

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(10000);

	/*----------------------
		SOCKET 연결
		-----------------------*/
	int iRet = connect(sock, (sockaddr*)&sa, sizeof(sa));

	u_long iMode = 1;
	/*----------------------언리얼 엔진 vector deleting destructor'() 에러 원인이 뭐야 : End시 Thread에 join하고 끝내야함 안하면 이지랄남
	SOCKET 논블럭킹 설정  | ioctlsocket
	-----------------------*/
	ioctlsocket(sock, FIONBIO, &iMode);

	rece_thread = std::thread(&UAZGameInstance::receive_thread, this);
	rece_queue_thread = std::thread(&UAZGameInstance::receive_data_read_thread, this);
	//rece_queue_move_info_thread = std::thread(&UTeemoGameInstance::receive_ingame_moveinfo_data_read_thread, this);

	client_check = true;
}

void UAZGameInstance::Client_Shutdown()
{
	UE_LOG(LogTemp, Warning, TEXT("[Client_Shutdown]\n"));
	recevie_connected = false;

	rece_thread.join();
	rece_queue_thread.join();
	//rece_queue_move_info_thread.join();

	closesocket(sock);
	WSACleanup();
}

void UAZGameInstance::Server_Packet_Send(const char* packet, int packet_size)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server_Packet_Send] sendData : %s size : %d\n"), packet, packet_size);

	send(sock, packet, packet_size, 0);
}

void UAZGameInstance::Signin()
{
	send(sock, (char*)&signin_packet, sizeof(signin_packet), 0);
}

void UAZGameInstance::receive_thread()
{
	char buffer[1024];
	int result;

	while (recevie_connected)
	{
		result = recv(sock, buffer, sizeof(buffer), 0);

		std::this_thread::sleep_for(std::chrono::milliseconds(100));

		// TODO 이곳에서 데이터 전송 받는거 확인하기

		if (result > 0)
		{
			Login_Send_Packet Login_data;
			ZeroMemory(&Login_data, sizeof(Login_data));
			CopyMemory(&Login_data, buffer, sizeof(Login_data));

			UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_thread] packet id : %d data : %s\n"), Login_data.packet_id, *defind.CharArrayToFString(Login_data.user_id));

			// 로그인 채팅 관련 패킷 : 400이하
			if (Login_data.packet_id <= 400)
			{
				std::lock_guard<std::mutex> lock(received_data_mutex);

				// 받은 데이터 큐에 밀어 넣기
				receive_header_check_data_queue.push(&Login_data);
			}
			// 인게임 관련 패킷 400이상
			else if (Login_data.packet_id > 400 && client_check)
			{
				FSetMoveInfo in_game_move_data_;
				ZeroMemory(&in_game_move_data_, sizeof(in_game_move_data_));
				CopyMemory(&in_game_move_data_, buffer, sizeof(in_game_move_data_));
				UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_thread else] packet id : %d size : %d fvector : %s frotator : %s\n"),
					in_game_move_data_.packet_id, in_game_move_data_.packet_length, *in_game_move_data_.fvector_.ToString(), *in_game_move_data_.frotator_.ToString());

				std::lock_guard<std::mutex> lock(received_data_mutex);

				// 받은 데이터 큐에 밀어 넣기
				receive_ingame_moveinfo_data_queue.push(&in_game_move_data_);
			}
		}
	}
}

void UAZGameInstance::receive_data_read_thread()
{
	while (recevie_connected)
	{
		// 대기열에 액세스하기 위한 잠금 획득
		std::lock_guard<std::mutex> lock(received_data_mutex);

		// 큐에 받은 데이터가 있는지 확인
		if (!receive_header_check_data_queue.empty()) {
			// 대기열에서 처음 받은 데이터 가져오기
			Login_Send_Packet* received_data = receive_header_check_data_queue.front();

			switch (received_data->packet_id)
			{
			case (UINT32)CLIENT_PACKET_ID::LOGIN_RESPONSE_SUCCESS:
				::MessageBox(NULL, L"Signin_Success", L"SignIn", 0);
				UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 201] packet id : %d\n"), received_data->packet_id);
				// 캐릭터 선택창 델리게이트 달어야하지만 인게임 진입으로 일단 변경
				if (Fuc_in_game_connect.IsBound() == true) Fuc_in_game_connect.Execute(received_data->clinet_id);
				break;
			case (UINT32)CLIENT_PACKET_ID::LOGIN_RESPONSE_FAIL:
				//::MessageBox(NULL, L"Signin_Fail", L"SignIn", 0);
				UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 202] packet id : %d\n"), received_data->packet_id);
				break;
			case (UINT32)CLIENT_PACKET_ID::SIGNIN_RESPONSE_SUCCESS:
				//::MessageBox(NULL, L"Signup_Success", L"Signup", 0);
				UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 203] packet id : %d\n"), received_data->packet_id);
				break;
			case (UINT32)CLIENT_PACKET_ID::SIGNIN_RESPONSE_FAIL:
				//::MessageBox(NULL, L"Signup_Fail", L"Signup", 0);
				UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 204] packet id : %d\n"), received_data->packet_id);
				break;
			case (UINT32)CLIENT_PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS:
				//::MessageBox(NULL, L"BroadCast Msg", L"Signup", 0);
				// TODO 여기에 델리게이트 달아서 챗 메세지 들어온거 확인하기                     
				UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 302] packet id : %d Data : %s\n"), received_data->packet_id, *defind.CharArrayToFString(received_data->user_id));
				if (Fuc_boradcast_success.IsBound() == true) Fuc_boradcast_success.Execute(*defind.CharArrayToFString(received_data->user_id));
				if (Fuc_Dynamic_OneParam.IsBound() == true) Fuc_Dynamic_OneParam.Broadcast(*defind.CharArrayToFString(received_data->user_id));
				break;
			default:
				UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch default] packet id : %d\n"), received_data->packet_id);
				break;
			}
			receive_header_check_data_queue.pop();
		}
	}
}

void UAZGameInstance::receive_ingame_moveinfo_data_read_thread()
{
}

void UAZGameInstance::ClientTimerProcessPacket()
{
	UE_LOG(LogTemp, Warning, TEXT("ClientTimerProcessPacket!"));

	// 대기열에 액세스하기 위한 잠금 획득
	std::lock_guard<std::mutex> lock(received_data_mutex);

	// 큐에 받은 데이터가 있는지 확인
	if (!receive_ingame_moveinfo_data_queue.empty()) {
		// 대기열에서 처음 받은 데이터 가져오기
		FSetMoveInfo* received_ingmae_data_ = receive_ingame_moveinfo_data_queue.front();

		switch (received_ingmae_data_->packet_id)
		{
		case (UINT32)CLIENT_PACKET_ID::IN_GAME_SUCCESS:
			::MessageBox(NULL, L"IN_GAME_SUCCESS", L"SignIn", 0);
			UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch 402] packet id : %d data : %s\n"), received_ingmae_data_->packet_id, *received_ingmae_data_->fvector_.ToString());
			// 캐릭터 선택창 델리게이트 달어야하지만 인게임 진입으로 일단 변경
			if (Fuc_in_game_init.IsBound() == true) Fuc_in_game_init.Execute(*received_ingmae_data_);
			break;
		case (UINT32)CLIENT_PACKET_ID::IN_GAME_MOVE_END:
			UE_LOG(LogTemp, Warning, TEXT("[move_info_3] packet id : %d vector : %s rotator : %s\n"),
				received_ingmae_data_->packet_id, *received_ingmae_data_->fvector_.ToString(), *received_ingmae_data_->frotator_.ToString());
			// 델리게이트 달아서 매니저로 보내자 일단
			if (Fun_move_info_.IsBound() == true) Fun_move_info_.Execute(*received_ingmae_data_);
			break;
		case (UINT32)CLIENT_PACKET_ID::MONSTER_RESPONSE: // add code
			UE_LOG(LogTemp, Warning, TEXT("[MONSTER_RESPONSE] packet id : %d vector : %s rotator : %s\n"),
				received_ingmae_data_->packet_id, *received_ingmae_data_->fvector_.ToString(), *received_ingmae_data_->frotator_.ToString());
			// 클라에서 해야할일 불라불라
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("[client_to_server_receive_switch default] packet id : %d\n"), received_ingmae_data_->packet_id);
			break;
		}
		receive_ingame_moveinfo_data_queue.pop();
	}

	if (server_client_check)
	{
		FTimerManager& timerManager = GetWorld()->GetTimerManager();
		timerManager.ClearTimer(client_timer_handle_);
	}
}

void UAZGameInstance::InGameAccept()
{
	UE_LOG(LogTemp, Warning, TEXT("[InGameAccept]\n"));

	Login_Send_Packet login_send_packet;
	login_send_packet.packet_id = (int)CLIENT_PACKET_ID::IN_GAME_REQUEST;
	login_send_packet.packet_length = sizeof(login_send_packet);

	Server_Packet_Send((char*)&login_send_packet, login_send_packet.packet_length);
}

void UAZGameInstance::Monster_Sample_Hello_World() // add code
{
	UE_LOG(LogTemp, Warning, TEXT("[Monster_Sample_Hello]\n"));

	// 서버로 보내고자 하는 패킷 정리
	SetMoveInfo monster_default_spawn; // 몬스터 스폰 초기 위치 값
	// 공통 사항 시작//
	monster_default_spawn.packet_id = (int)CLIENT_PACKET_ID::MONSTER_REQUEST; // 몬스터 생성 요청 packet id
	monster_default_spawn.packet_length = sizeof(monster_default_spawn); // 몬스터 생성 요청 패킷 총 크기
	// 공통 사항 끝//

	// 서버에 전달 하고자 하는 패킷 내용 
	monster_default_spawn.fvector_ = FVector(100.0f, 0.0f, 0.0f); // 몬스터 초기 위치값 x : 100 y : 0 z : 0
	monster_default_spawn.frotator_ = FRotator(0.0f, 0.0f, 0.0f); // 몬스터 초기 회전값 x : 0 y : 0 z : 0

	// 보내고자 한 내용을 패킷에 포장했기 때문에 서버로 전송하기
	Server_Packet_Send((char*)&monster_default_spawn, monster_default_spawn.packet_length); // Server_Packet_Send((char*)&보내고자하는패킷 , 보내고자하는 패킷 사이즈)

	// 정리
	// 서버로 601 요청과 함께 FVector와 FRotator를 함께 보냈다
}

// client end