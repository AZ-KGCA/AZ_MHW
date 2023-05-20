#pragma once
#define WIN32_LEAN_AND_MEAN
#include "Windows/AllowWindowsPlatformTypes.h"
#include <Windows.h>
#include "Windows/HideWindowsPlatformTypes.h"
#include "Odbc.h"

struct RawPacketData
{
	UINT32 client_index_ = 0;
	UINT32 data_size_ = 0;
	char* P_packet_data_ = nullptr;

	void Set(RawPacketData& vlaue)
	{
		client_index_ = vlaue.client_index_;
		data_size_ = vlaue.data_size_;

		P_packet_data_ = new char[vlaue.data_size_];
		CopyMemory(P_packet_data_, vlaue.P_packet_data_, vlaue.data_size_);
	}

	void Set(UINT32 client_index, UINT32 data_size, char* P_data)
	{
		client_index_ = client_index;
		data_size_ = data_size;

		P_packet_data_ = new char[data_size];
		CopyMemory(P_packet_data_, P_data, data_size);
	}

	void Release()
	{
		delete P_packet_data_;
	}
};

struct PacketInfo
{
	UINT32 client_index_ = 0;
	UINT16 packet_id_ = 0;
	UINT16 data_size_ = 0;
	char* P_data_ptr_ = nullptr;
};

enum class PACKET_ID : UINT16
{
	// TODO 오늘 수정해야함

	//SYSTEM
	SYS_USER_CONNECT = 11,
	SYS_USER_DISCONNECT = 12,
	SYS_END = 30,

	//DB
	DB_END = 99,

	// Server -> Client

	// Client -> Server
	LOGIN_REQUEST = 201,
	LOGIN_RESPONSE_SUCCESS = 202,
	LOGIN_RESPONSE_FAIL = 203,

	SIGNIN_REQUEST = 204,
	SIGNIN_RESPONSE_SUCCESS = 205,
	SIGNIN_RESPONSE_FAIL = 206,

	CHAT_SEND_REQUEST = 301,
	CHAT_SEND_RESPONSE_SUCCESS = 302,
	CHAT_SEND_RESPONSE_FAIL = 303,

	IN_GAME_REQUEST = 401,
	IN_GAME_SUCCESS = 402,
	IN_GAME_FAIL = 403,

	IN_GAME_INPUT_REQUEST = 501,
	IN_GAME_INPUT_RESPONSE = 502,

	//ROOM_ENTER_REQUEST = 206,
	//ROOM_ENTER_RESPONSE = 207,

	//ROOM_LEAVE_REQUEST = 215,
	//ROOM_LEAVE_RESPONSE = 216,

	//ROOM_CHAT_REQUEST = 221,
	//ROOM_CHAT_RESPONSE = 222,
	//ROOM_CHAT_NOTIFY = 223,

	PACKET_CHATNAME_REQ = 1001,
};

#pragma pack(push,1)
struct PACKET_HEADER
{
	UINT16 packet_length_;
	UINT16 packet_id_;
	//UINT8 type_; //압축여부 암호화여부 등 속성을 알아내는 값
};



const UINT32 PACKET_HEADER_LENGTH = sizeof(PACKET_HEADER);

// 로그인 요청
const int MAX_USER_ID_LEN = 32;
const int MAX_USER_PW_LEN = 32;

struct LOGIN_REQUEST_PACKET : public PACKET_HEADER
{
	char user_id_[MAX_USER_ID_LEN + 1];
	char user_pw_[MAX_USER_PW_LEN + 1];
};

const size_t LOGIN_REQUEST_PACKET_SZIE = sizeof(LOGIN_REQUEST_PACKET);

struct LOGIN_RESPONSE_PACKET : public PACKET_HEADER
{
	UINT16 result_;
};

//- 룸에 들어가기 요청
//const int MAX_ROOM_TITLE_SIZE = 32;
struct ROOM_ENTER_REQUEST_PACKET : public PACKET_HEADER
{
	INT32 room_number_;
};
struct ROOM_ENTER_RESPONSE_PACKET : public PACKET_HEADER
{
	INT16 result_;
	//char RivalUserID[MAX_USER_ID_LEN + 1] = { 0, };
};

//- 룸 나가기 요청
struct ROOM_LEAVE_REQUEST_PACKET : public PACKET_HEADER
{
};

struct ROOM_LEAVE_RESPONSE_PACKET : public PACKET_HEADER
{
	INT16 result_;
};

// 룸 채팅
const int MAX_CHAT_MSG_SIZE = 256;
struct ROOM_CHAT_REQUEST_PACKET : public PACKET_HEADER
{
	char message_[MAX_CHAT_MSG_SIZE + 1] = { 0, };
};

struct ROOM_CHAT_RESPONSE_PACKET : public PACKET_HEADER
{
	INT16 result_;
};

struct ROOM_CHAT_NOTIFY_PACKET : public PACKET_HEADER
{
	char user_id_[MAX_USER_ID_LEN + 1] = { 0, };
	char msg_[MAX_CHAT_MSG_SIZE + 1] = { 0, };
};
#pragma pack(pop) //위에 설정된 패킹설정이 사라짐

class Packet
{
};