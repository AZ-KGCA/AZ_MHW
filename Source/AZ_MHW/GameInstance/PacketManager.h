#pragma once

#include "Packet.h"
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>
//#include "Odbc.h"

#include <unordered_map>
#include <deque>
#include <functional>
#include <thread>
#include <mutex>

class UserManager;
class RoomManger;
class RedisManager;


class PacketManager
{
public:
	PacketManager() = default;
	~PacketManager() = default;

	void Init(const UINT32 max_client);

	bool Run();

	void End();

	void ReceivePacketData(const UINT32 client_index, const UINT32 size, char* P_data);

	void PushSystemPacket(PacketInfo packet);

	void DbRun();

	std::function<void(UINT32, UINT32, char*)> SendPacketFunc;
	std::function<void(UINT32, UINT32, char*)> BroadCastSendPacketFunc;

	SQLTCHAR* ConvertCharToSqlTCHAR(const char* charArray);

public:
	Odbc odbc;

private:
	void CreateCompent(const UINT32 max_client);

	void ClearConnectionInfo(INT32 client_index);

	void EnqueuePacketData(const UINT32 client_index);

	PacketInfo DequePacketData();

	PacketInfo DequeSystemPacketData();

	void ProcessPacket();

	void ProcessRecvPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet);

	void ProcessuserConnect(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessUserDisConnect(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessLogin(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessSignup(UINT32 client_index, UINT16 packet_size, char* P_packet);

	void ProcessChatting(UINT32 client_index, UINT16 packet_size, char* P_packet);

	typedef void (PacketManager::* PROCESS_RECV_PACKET_FUNCTION)(UINT32, UINT16, char*);

	std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION> recv_funtion_dictionary_;

	UserManager* user_manager_;

	std::function<void(int, char*)> send_mq_data_func_;

	bool is_run_process_thread_ = false;

	std::thread process_thread_;

	std::mutex lock_;

	// ���� �����Ͱ� ������ ��� �ϴ� ť
	std::deque<UINT32> in_coming_packet_user_index_;

	// ��Ʈ��ũ ���� & �������� ó���ϴ� ť
	std::deque<PacketInfo> system_packet_queue_;
};

