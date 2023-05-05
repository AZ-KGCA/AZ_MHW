#pragma once
#include "IocpNetServer.h"
#include "Packet.h"
#include "PacketManager.h"

#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <memory>

class AppServer : public IocpNetServer
{
public:
	AppServer() = default;
	virtual ~AppServer() = default;

	virtual void OnConnect(const UINT32 client_index) override
	{
		//printf("[OnConnect] Ŭ���̾�Ʈ: Index(%d)\n", client_index);
		UE_LOG(LogTemp, Warning, TEXT("[OnConnect] Client : Index(%d)\n"), client_index);
		// PacketInfo ����
		// UINT32 client_index_ = 0;
		// UINT16 packet_id_ = 0;
		// UINT16 data_size_ = 0;
		PacketInfo packet{ client_index, (UINT16)PACKET_ID::PACKET_CHATNAME_REQ, 0 };

		// SYS_USER_CONNECT ��Ŷ�� ����
		P_packet_manager_->PushSystemPacket(packet);
	}

	virtual void OnClose(const UINT32 client_index_) override
	{
		//printf("[OnClose] Ŭ���̾�Ʈ: Index(%d)\n", client_index_);
		UE_LOG(LogTemp, Warning, TEXT("[OnClose] Client : Index(%d)\n"), client_index_);

		PacketInfo packet{ client_index_, (UINT16)PACKET_ID::SYS_USER_DISCONNECT, 0 };
		P_packet_manager_->PushSystemPacket(packet);
	}

	virtual void OnReceive(const UINT32 client_index, const UINT32 size, char* P_recv_data) override
	{
		//printf("[OnReceive] Ŭ���̾�Ʈ: Index(%d), dataSize(%d), recvData : %s\n", client_index, size, P_recv_data);

		UE_LOG(LogTemp, Warning, TEXT("[OnReceive] Client : Index(%d), dataSize(%d), recvData : %s\n"), client_index, size, P_recv_data);

		P_packet_manager_->ReceivePacketData(client_index, size, P_recv_data);

		//Receive ȣ�� Send ȣ���ؼ� ���� ���� ���·� ����
		//P_packet_manager_->SendPacketFunc(client_index, size, P_recv_data);
		// ���� ȣ��
		//SendMsg(client_index, size, P_recv_data);
	}

	void Run(const UINT32 max_client)
	{
		// ���ٷ� funtion ��ü�� SendMsg ����
		auto send_packet_func = [&](UINT32 client_index, UINT16 packet_size, char* P_send_packet)
		{
			SendMsg(client_index, packet_size, P_send_packet);
		};

		auto boradcast_send_packet_func = [&](UINT32 client_index, UINT16 packet_size, char* P_send_packet)
		{
			BroadcastSendMsg(client_index, packet_size, P_send_packet);
		};



		// ��Ŷ �Ŵ��� ����
		P_packet_manager_ = std::make_unique<PacketManager>();
		P_packet_manager_->SendPacketFunc = send_packet_func;
		P_packet_manager_->BroadCastSendPacketFunc = boradcast_send_packet_func;

		// Ŭ���̾�Ʈ �� ��ŭ user ���� index, packet_data_buffer �ʱ�ȭ
		P_packet_manager_->Init(max_client);

		// ��Ŷ������ ����
		P_packet_manager_->Run();

		// DB ����
		P_packet_manager_->DbRun();

		StartServer(max_client);
	}

	void End()
	{
		P_packet_manager_->End();

		DestroyThread();
	}

private:
	std::unique_ptr<PacketManager> P_packet_manager_;
};
