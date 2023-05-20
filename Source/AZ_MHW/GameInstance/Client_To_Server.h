// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"

#include <iostream>
#include <winsock2.h>
#include "Windows/AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include <thread>
#include <queue>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

#include "Client_Packet.h"
#include "InGamePacket.h"

#include "UObject/NoExportTypes.h"
#include "Client_To_Server.generated.h"

// SingleCast
DECLARE_DELEGATE(FDle_InGameConnect);

//Dynamic
DECLARE_DELEGATE_OneParam(FDle_InGameInit, const FSetMoveInfo&);
DECLARE_DELEGATE_OneParam(FChat_Broadcast_Success, const FString&);

/**
 * 
 */
UCLASS()
class AZ_MHW_API UClient_To_Server : public UObject
{
	GENERATED_BODY()
public:	// Delegate
	FDle_InGameConnect Fuc_in_game_connect;

	FDle_InGameInit Fuc_in_game_init;
	FChat_Broadcast_Success Fuc_boradcast_success;

public:
	UClient_To_Server();
	~UClient_To_Server();

	void Server_Connect();
	bool Connect(const FString& ip, int32 port);

	void Client_Shutdown();

	int Server_Packet_Send(const char* packet, int packet_size);

	void Signin();

	void receive_thread();
	void receive_data_read_thread();
	void receive_ingame_moveinfo_data_read_thread();

	// 캐릭터 동기화 초기 위치
	void InGameAccept();
public:
	SOCKET sock;
	SOCKADDR_IN sa; // 목적지+포트
	Login_Send_Packet signin_packet;

	std::thread rece_thread;
	std::thread rece_queue_thread;
	std::thread rece_queue_move_info_thread;

	bool recevie_connected = true;

	// Define a queue to store the received data
	std::queue<Login_Send_Packet*> receive_header_check_data_queue;
	std::queue<FSetMoveInfo*> receive_ingame_moveinfo_data_queue;

	// Define a mutex to ensure thread-safe access to the queue
	std::mutex received_data_mutex;

	Defind defind;

	bool client_check = false;

	// 캐릭터 동기화
	FSetMoveInfo set_move_info_;
};

