#pragma once
#include "IocpNetServer.h"
#include "Packet.h"
#include "PacketManager.h"

#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <memory>

const UINT16 SERVER_PORT = 10000;
const UINT16 MAX_CLIENT = 5;		//총 접속할수 있는 클라이언트 수
const int SLEEP_TIME = 3000;
const UINT32 MAX_IO_WORKER_THREAD = 4;

class AppServer : public IocpNetServer
{
public:
	AppServer() = default;
	virtual ~AppServer() = default;

	virtual void OnConnect(const UINT32 client_index) override
	{
		//printf("[OnConnect] 클라이언트: Index(%d)\n", client_index);
		UE_LOG(LogTemp, Warning, TEXT("[OnConnect] Client : Index(%d)\n"), client_index);
		// PacketInfo 정보
		// UINT32 client_index_ = 0;
		// UINT16 packet_id_ = 0;
		// UINT16 data_size_ = 0;
		PacketInfo packet{ client_index, (UINT16)PACKET_ID::PACKET_CHATNAME_REQ, 0 };

		// SYS_USER_CONNECT 패킷을 전달
		P_packet_manager_->PushSystemPacket(packet);
	}

	virtual void OnClose(const UINT32 client_index_) override
	{
		//printf("[OnClose] 클라이언트: Index(%d)\n", client_index_);
		UE_LOG(LogTemp, Warning, TEXT("[OnClose] Client : Index(%d)\n"), client_index_);

		PacketInfo packet{ client_index_, (UINT16)PACKET_ID::SYS_USER_DISCONNECT, 0 };
		P_packet_manager_->PushSystemPacket(packet);
	}

	virtual void OnReceive(const UINT32 client_index, const UINT32 size, char* P_recv_data) override
	{
		//printf("[OnReceive] 클라이언트: Index(%d), dataSize(%d), recvData : %s\n", client_index, size, P_recv_data);

		UE_LOG(LogTemp, Warning, TEXT("[OnReceive] Client : Index(%d), dataSize(%d), recvData : %s\n"), client_index, size, P_recv_data);

		P_packet_manager_->ReceivePacketData(client_index, size, P_recv_data);

		//Receive 호출 Send 호출해서 에코 서버 형태로 만듬
		//P_packet_manager_->SendPacketFunc(client_index, size, P_recv_data);
		// 에코 호출
		//SendMsg(client_index, size, P_recv_data);
	}

	void Run(const UINT32 max_client)
	{
		// 람다로 funtion 객체에 SendMsg 담음
		auto send_packet_func = [&](UINT32 client_index, UINT16 packet_size, char* P_send_packet)
		{
			SendMsg(client_index, packet_size, P_send_packet);
		};

		auto boradcast_send_packet_func = [&](UINT32 client_index, UINT16 packet_size, char* P_send_packet)
		{
			BroadcastSendMsg(client_index, packet_size, P_send_packet);
		};

		// 패킷 매니저 생성
		P_packet_manager_ = std::make_unique<PacketManager>();
		P_packet_manager_->SendPacketFunc = send_packet_func;
		P_packet_manager_->BroadCastSendPacketFunc = boradcast_send_packet_func;

		// 클라이언트 수 만큼 user 생성 index, packet_data_buffer 초기화
		//P_packet_manager_->Init(max_client);

		//// 패킷쓰레드 생성
		//P_packet_manager_->Run();

		//// DB 접속
		//P_packet_manager_->DbRun();

		//StartServer(max_client);
	}

	void End()
	{
		P_packet_manager_->End();

		DestroyThread();
	}

	void ServerStart();
public:
	bool server_check_ = false;

private:
	std::unique_ptr<PacketManager> P_packet_manager_;
};

