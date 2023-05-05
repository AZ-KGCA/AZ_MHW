// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <thread>
#include <queue>
#include <mutex>

#pragma comment(lib, "ws2_32.lib")

#include "Client_Packet.h"

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Client_To_Server.generated.h"

// SingleCast
DECLARE_DELEGATE(FDle_InGameConnect);

//Dynamic
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

	FChat_Broadcast_Success Fuc_boradcast_success;


public:
	UClient_To_Server();

	void Socket_Init();

	void Server_Connect();

	void Client_Shutdown();

	void Server_Packet_Send(const char* packet, int packet_size);

	void Signin();

	void receive_thread();

	void receive_data_read_thread();

public:
	SOCKET sock;
	SOCKADDR_IN sa; // 목적지+포트
	Login_Send_Packet signin_packet;

	std::thread rece_thread;
	std::thread rece_queue_thread;
	std::thread test_thread;

	bool recevie_connected = true;

	// Define a queue to store the received data
	std::queue<Login_Send_Packet*> receive_header_check_data_queue;

	// Define a mutex to ensure thread-safe access to the queue
	std::mutex received_data_mutex;

	Defind defind;

	bool client_check = false;
};

