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

#include  "Engine/GameInstance.h"
//FIXME ���ս� ����
#include <GameFramework/Character.h>

//MinSuhong Add
#include "Networking.h"
#include "Sockets.h"
#include "SocketSubsystem.h"
#include "Interfaces/IPv4/IPv4Address.h"
#include "Engine/Engine.h"

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

	AddNewSingleton(map_mgr = NewObject<UAZMapMgr>(this));
	msg_handler->OnRegister(map_mgr);

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UAZGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAZGameInstance::EndLoadingScreen);

	if (client_connect == nullptr)
	{
		client_connect = NewObject<UClient_To_Server>(this, TEXT("client_to_server"));
	}

	UE_LOG(LogTemp, Warning, TEXT("Start Client!"));
}

void UAZGameInstance::Shutdown()
{
	Super::Shutdown();

	UE_LOG(LogTemp, Warning, TEXT("Shutdown!"));

	if (socket_type == 0)
	{
		fsocket_version->Close();
	}
	else if (socket_type == 1 || socket_type == 2)
	{
		closesocket(win_socket);
	}
	else if (socket_type == 3)
	{
		iocp_net_server_.End();
	}
	else if (socket_type == 4)
	{
		g_odbc.DisConnect();
	}
	else if (socket_type == 5)
	{
		iocp_net_server_.End();
		g_odbc.DisConnect();
	}
	else if (socket_type == 6)
	{
		client_connect->Client_Shutdown();
	}
	else
	{

	}

	SendLogoutCmd();

	mgrs.Reset();
	AZ_LOG("Shutdown");

	// ������ �������� �Ҹ�
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

void UAZGameInstance::CreateSocketHolder()
{
	// FIXME ���ս� ���ϻ���
}

void UAZGameInstance::DestroySocketHolder()
{
	// FIXME ���ս� ���ϻ���
}

void UAZGameInstance::InitSocketOnMapLoad()
{
	// FIXME ���ս� �����ʱ�ȭ
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
	// FIXME ���ս� ��ġ��
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

void UAZGameInstance::PacketToServer(const char* packet, int packet_size)
{
	client_connect->Server_Packet_Send(packet, packet_size);
}

bool UAZGameInstance::LoginRecord(FString login_id, FString login_pw)
{
	dbitem record;

	// ���̵�
	SQLTCHAR name[255];
	std::string login_id_string = TCHAR_TO_ANSI(*login_id);
	std::wstring wstring_id(login_id_string.begin(), login_id_string.end());
	wcscpy_s(name, 255, wstring_id.c_str());

	// �н�����
	SQLTCHAR pw[255];
	std::string login_pw_string = TCHAR_TO_ANSI(*login_pw);
	std::wstring wstring_pw(login_pw_string.begin(), login_pw_string.end());
	wcscpy_s(pw, 255, wstring_pw.c_str());

	if (g_odbc.LoginCheckSQL(name, pw))
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Success"));

		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Login Fail"));

		return false;
	}
}

bool UAZGameInstance::SignupRecord(FString signup_id, FString signup_pw)
{
	dbitem record;

	// ���̵�
	SQLTCHAR name[255];
	std::string signup_id_string = TCHAR_TO_ANSI(*signup_id);
	std::wstring wstring_signup_id(signup_id_string.begin(), signup_id_string.end());
	wcscpy_s(name, 255, wstring_signup_id.c_str());

	// �н����� 1
	SQLTCHAR pw[255];
	std::string signup_pw_string = TCHAR_TO_ANSI(*signup_pw);
	std::wstring wstring_signup_pw(signup_pw_string.begin(), signup_pw_string.end());
	wcscpy_s(pw, 255, wstring_signup_pw.c_str());

	record.name = name;
	record.pass = pw;

	if (g_odbc.AddSQL(record))
	{
		UE_LOG(LogTemp, Warning, TEXT("Signup Success"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Signup Fail"));
	}

	return false;
}

void UAZGameInstance::IocpServerStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Iocp & Mssql Open\n"));
	//������ �ʱ�ȭ
	iocp_net_server_.Init(MAX_IO_WORKER_THREAD);

	//���ϰ� ���� �ּҸ� �����ϰ� ��� ��Ų��.
	iocp_net_server_.BindAndListen(SERVER_PORT);

	iocp_net_server_.Run(MAX_CLIENT);
}

bool UAZGameInstance::ClientSignin()
{
	return false;
}

void UAZGameInstance::FSocketConncet()
{
	socket_type = 0;

	// ������ ����
	fsocket_version = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(TEXT("Stream"), TEXT("ClientSocket"));

	// IP�� FString���� �Է¹޾� ����
	FString address = TEXT("127.0.0.1");
	FIPv4Address ip;
	FIPv4Address::Parse(address, ip);

	int32 port = 10000;	// ��Ʈ�� 6000��

	// ��Ʈ�� ������ ��� Ŭ����
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(port);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Trying to connect.")));

	fsocket_version->Connect(*addr);
}

void UAZGameInstance::WinSocketConnect()
{
	socket_type = 1;

	int32 nRet = WSAStartup(MAKEWORD(2, 2), &wsa);  // Winsocket �ʱ�ȭ

	win_socket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa; // ������ + ��Ʈ

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = inet_addr("127.0.0.1");
	sa.sin_port = htons(10000);

	connect(win_socket, (sockaddr*)&sa, sizeof(sa));
}

void UAZGameInstance::WinSocketOpen()
{
	UE_LOG(LogTemp, Log, TEXT("WinSocket Accept"));
	socket_type = 2;

	/*----------------------
	SOCKET ����
	-----------------------*/

	win_socket = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN sa; // ������ + ��Ʈ

	sa.sin_family = AF_INET;
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	sa.sin_port = htons(10000);

	/*----------------------
	SOCKET ���ε�
	-----------------------*/
	int iRet = bind(win_socket, (sockaddr*)&sa, sizeof(sa));

	/*----------------------
	SOCKET ���� ���
	-----------------------*/
	iRet = listen(win_socket, SOMAXCONN);

	// ���ӵǸ� ��ȯ�ȴ�.
	SOCKADDR_IN clientaddr;
	int length = sizeof(clientaddr);

	/*----------------------
	SOCKET ���� ����
	-----------------------*/
	SOCKET clientSock = accept(win_socket, (sockaddr*)&clientaddr, &length);
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("Hello\n"));

	char szRecvMsg[256] = { 0, };
	/*----------------------
	SOCKET ���� ������ �ޱ�
	-----------------------*/
	int iRecvBytes = recv(clientSock, szRecvMsg, 256, 0);
	printf("%s\n", szRecvMsg);

	/*----------------------
	SOCKET ���� ������ ������
	-----------------------*/
	int iSendBytes = send(clientSock, szRecvMsg, strlen(szRecvMsg), 0);

	closesocket(win_socket);
}

void UAZGameInstance::IocpServerOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("IocpServerOpen\n"));
	socket_type = 3;
	//������ �ʱ�ȭ
	iocp_net_server_.Init(MAX_IO_WORKER_THREAD);

	//���ϰ� ���� �ּҸ� �����ϰ� ��� ��Ų��.
	iocp_net_server_.BindAndListen(SERVER_PORT);

	iocp_net_server_.Run(MAX_CLIENT);
}

void UAZGameInstance::OdbcConnect()
{
	UE_LOG(LogTemp, Warning, TEXT("OdbcConnect\n"));
	socket_type = 4;
	g_odbc.Init();
	g_odbc.ConnetMssql(L"odbc_test.dsn");
	g_odbc.Load();
}

void UAZGameInstance::IocpOdbcOpen()
{
	UE_LOG(LogTemp, Warning, TEXT("Iocp & Odbc\n"));
	socket_type = 5;
	IocpServerStart();
}

void UAZGameInstance::ServerConnect()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerConnect\n"));
	socket_type = 6;

	if (client_connect == nullptr)
	{
		client_connect = NewObject<UClient_To_Server>(this, TEXT("client_to_server"));
	}

	client_connect->Socket_Init();
	client_connect->Server_Connect();
}
