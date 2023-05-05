#include "PacketManager.h"
#include <utility>
#include <cstring>
#include "UserManager.h"
#include "AppServer.h"
#include <Windows.h>
#include <sqltypes.h>

#include "Client_Packet.h"

void PacketManager::Init(const UINT32 max_client)
{
	// unordered_map
	// https://blog.naver.com/PostView.nhn?blogId=webserver3315&logNo=221678909965&parentCategoryNo=&categoryNo=48&viewDate=&isShowPopularPosts=true&from=search
	recv_funtion_dictionary_ = std::unordered_map<int, PROCESS_RECV_PACKET_FUNCTION>();

	// (PACKET_ID ����) Key, Value ����
	recv_funtion_dictionary_[(int)PACKET_ID::SYS_USER_CONNECT] = &PacketManager::ProcessuserConnect;
	recv_funtion_dictionary_[(int)PACKET_ID::SYS_USER_DISCONNECT] = &PacketManager::ProcessUserDisConnect;
	recv_funtion_dictionary_[(int)PACKET_ID::LOGIN_REQUEST] = &PacketManager::ProcessLogin;
	recv_funtion_dictionary_[(int)PACKET_ID::SIGNIN_REQUEST] = &PacketManager::ProcessSignup;
	recv_funtion_dictionary_[(int)PACKET_ID::CHAT_SEND_REQUEST] = &PacketManager::ProcessChatting;

	CreateCompent(max_client);
}

void PacketManager::CreateCompent(const UINT32 max_client)
{
	user_manager_ = new UserManager;
	user_manager_->Init(max_client);
}

bool PacketManager::Run()
{
	//�� �κ��� ��Ŷ ó�� �κ����� �̵� ��Ų��.
	is_run_process_thread_ = true;
	// ��Ŷ ������ ����
	process_thread_ = std::thread([this]() {ProcessPacket(); });

	return true;
}

void PacketManager::End()
{
	is_run_process_thread_ = false;

	if (process_thread_.joinable())
	{
		process_thread_.join();
	}
}

void PacketManager::ReceivePacketData(const UINT32 client_index, const UINT32 size, char* P_data)
{
	// GetUserByConnIdx � Ŭ���̾�Ʈ�� �޼����� ���´��� Ȯ��
	auto P_user = user_manager_->GetUserByConnIdx(client_index);

	// �޼����� ���� Ŭ���̾�Ʈ ��ü�� �����͸� ����
	P_user->SetPacketData(size, P_data);

	// ť�� ���ú갡 �߻��ߴٴ� ���� �˷���
	EnqueuePacketData(client_index);
}

void PacketManager::ProcessPacket()
{
	while (is_run_process_thread_)
	{
		bool is_idle = true;

		// �̹� ������ �� ������ ���� ��û�� �ִ� ���
		if (auto packet_data = DequePacketData(); packet_data.packet_id_ > (UINT16)PACKET_ID::SYS_END)
		{
			is_idle = false;
			// ��û�� �ִ� ��� ó��
			ProcessRecvPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
		}

		// �ý��� ��Ŷ (���� & ���� ���ᰡ �߻��� ���)
		if (auto packet_data = DequeSystemPacketData(); packet_data.packet_id_ != 0)
		{
			is_idle = false;
			// ��û�� �ִ� ��� ó��
			ProcessRecvPacket(packet_data.client_index_, packet_data.packet_id_, packet_data.data_size_, packet_data.P_data_ptr_);
		}

		if (is_idle)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
	}
}

void PacketManager::EnqueuePacketData(const UINT32 client_index)
{
	std::lock_guard<std::mutex> guard(lock_);
	in_coming_packet_user_index_.push_back(client_index);
}

PacketInfo PacketManager::DequePacketData()
{
	UINT32 user_index = 0;

	{
		std::lock_guard<std::mutex> grard(lock_);
		// ���� Send ��û�� ���� ������ �ִ��� Ȯ��
		if (in_coming_packet_user_index_.empty())
		{
			return PacketInfo();
		}

		// ��û�� �����Ͱ� �ִٸ� index ����
		user_index = in_coming_packet_user_index_.front();
		in_coming_packet_user_index_.pop_front();
	}

	// �˾Ƴ� index�� ���� ��ü ������
	auto P_user = user_manager_->GetUserByConnIdx(user_index);
	auto packet_data = P_user->GetPacket();
	packet_data.client_index_ = user_index;

	return packet_data;
}

void PacketManager::PushSystemPacket(PacketInfo packet)
{
	std::lock_guard<std::mutex> guard(lock_);
	// ��Ŷ ����� ���� ó��
	system_packet_queue_.push_back(packet);
}

void PacketManager::DbRun()
{
	odbc.Init();
	odbc.ConnetMssql(L"odbc_test.dsn");
	odbc.Load();
}

SQLTCHAR* PacketManager::ConvertCharToSqlTCHAR(const char* charArray)
{
	int charArrayLen = strlen(charArray) + 1;
	int numChars = MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, NULL, 0);

	SQLTCHAR* sqlTCharArray = new SQLTCHAR[numChars];

	MultiByteToWideChar(CP_ACP, 0, charArray, charArrayLen, (LPWSTR)sqlTCharArray, numChars);

	return sqlTCharArray;
}

PacketInfo PacketManager::DequeSystemPacketData()
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

void PacketManager::ProcessRecvPacket(const UINT32 client_index, const UINT16 packet_id, const UINT16 packet_size, char* P_packet)
{
	// ��û�� ��Ŷ id�� Ŭ���̾�Ʈ ��ü ã��
	auto iter = recv_funtion_dictionary_.find(packet_id);
	if (iter != recv_funtion_dictionary_.end())
	{
		// ã�� ��ü�� ó��
		(this->*(iter->second))(client_index, packet_size, P_packet);
	}
}

void PacketManager::ProcessuserConnect(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	// ���� ���� �ޱ�
	printf("[ProcessUserConnect] clientIndex: %d\n", client_index);
	// �ϳ��� user_manger ��ü �Ҵ�
	auto P_user = user_manager_->GetUserByConnIdx(client_index);
	// ��ü �ʱ�ȭ
	P_user->Clear();
}

void PacketManager::ProcessUserDisConnect(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	printf("[ProcessUserDisConnect] clientIndex: %d\n", client_index);
	// ������ ������ ��� user ��ȯ
	ClearConnectionInfo(client_index);
}

// �α��� ���μ���
void PacketManager::ProcessLogin(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	auto  P_login_req_packet = reinterpret_cast<LOGIN_REQUEST_PACKET*>(P_packet);

	auto P_user_id = ConvertCharToSqlTCHAR(P_login_req_packet->user_id_);
	auto P_user_pw = ConvertCharToSqlTCHAR(P_login_req_packet->user_pw_);
	UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	if (odbc.LoginCheckSQL(P_user_id, P_user_pw))
	{

		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		header_check_packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::LOGIN_RESPONSE_SUCCESS;
		login_res_packet.packet_length = sizeof(login_res_packet);

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		header_check_packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::LOGIN_RESPONSE_FAIL;
		login_res_packet.packet_length = sizeof(login_res_packet);

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}

	// TODO. �ߺ� �α��� �ȵǴ� ���
	//if (user_manager_->GetCurrentUserCnt() >= user_manager_->GetMaxUserCnt())
	//{
	//	//�����ڼ��� �ִ���� �����ؼ� ���ӺҰ�
	//	login_res_packet.result_ = (UINT16)ERROR_CODE::LOGIN_USER_USED_ALL_OBJ;
	//	SendPacketFunc(client_index, sizeof(LOGIN_RESPONSE_PACKET), (char*)&login_res_packet);
	//	return;
	//}

	// TODO. ��� ���� �ο����� ������ ����
	////���⿡�� �̹� ���ӵ� �������� Ȯ���ϰ�, ���ӵ� ������� �����Ѵ�.
	//if (user_manager_->FindUserIndexByID(P_user_id) == -1)
	//{
	//	login_res_packet.result_ = (UINT16)ERROR_CODE::NONE;
	//	SendPacketFunc(client_index, sizeof(LOGIN_RESPONSE_PACKET), (char*)&login_res_packet);
	//}
	//else
	//{
	//	//�������� �������� ���и� ��ȯ�Ѵ�.
	//	login_res_packet.result_ = (UINT16)ERROR_CODE::LOGIN_USER_ALREADY;
	//	SendPacketFunc(client_index, sizeof(LOGIN_REQUEST_PACKET), (char*)&login_res_packet);
	//	return;
	//}
}

void PacketManager::ProcessSignup(UINT32 client_index, UINT16 packet_size, char* P_packet)
{

	dbitem record;

	auto  P_signup_req_packet = reinterpret_cast<LOGIN_REQUEST_PACKET*>(P_packet);

	auto P_user_id = ConvertCharToSqlTCHAR(P_signup_req_packet->user_id_); ;
	auto P_user_pw = ConvertCharToSqlTCHAR(P_signup_req_packet->user_pw_); ;

	UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	record.name = P_user_id;
	record.pass = P_user_pw;

	//UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup] Id : %s / PW : %s\n"), record.name, record.pass);
	///////////////////////////

	if (odbc.AddSQL(record))
	{
		//odbc.Load();

		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		header_check_packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::SIGNIN_RESPONSE_SUCCESS;
		login_res_packet.packet_length = sizeof(login_res_packet);

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		header_check_packet login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::SIGNIN_RESPONSE_FAIL;
		login_res_packet.packet_length = sizeof(login_res_packet);

		SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
}

void PacketManager::ProcessChatting(UINT32 client_index, UINT16 packet_size, char* P_packet)
{
	Defind defind;
	auto  P_login_req_packet = reinterpret_cast<Login_Send_Packet*>(P_packet);

	Login_Send_Packet login_res_packet;
	login_res_packet.packet_id = (int)PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS;
	login_res_packet.packet_length = sizeof(login_res_packet);
	strcpy(login_res_packet.user_id, P_login_req_packet->user_id);

	UE_LOG(LogTemp, Warning, TEXT("[ProcessChatting Send] packet_id : %d, packet_length : %d packet_data %s\n"),
		login_res_packet.packet_id, login_res_packet.packet_length, *defind.CharArrayToFString(login_res_packet.user_id));

	BroadCastSendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	//SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
}

void PacketManager::ClearConnectionInfo(INT32 client_index)
{
	auto P_req_user = user_manager_->GetUserByConnIdx(client_index);

	if (P_req_user->GetDomainState() != User::DOMAIN_STATE::NONE)
	{
		user_manager_->DeleteUserInfo(P_req_user);
	}
}