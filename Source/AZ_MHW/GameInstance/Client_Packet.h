#pragma once
#include "Define.h"

struct Login_Send_Packet
{
	unsigned short packet_length;
	unsigned short packet_id;
	//bool packet_type;
	char user_id[33];
	char user_pw[33];
};

struct header_check_packet
{
	unsigned short packet_length;
	unsigned short packet_id;

	char user_id[33];
	char user_pw[33];
	//char* pDataPtr = nullptr;
	//bool packet_type;
};

enum class CLIENT_PACKET_ID : UINT16
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

	//ROOM_ENTER_REQUEST = 206,
	//ROOM_ENTER_RESPONSE = 207,

	//ROOM_LEAVE_REQUEST = 215,
	//ROOM_LEAVE_RESPONSE = 216,

	//ROOM_CHAT_REQUEST = 221,
	//ROOM_CHAT_RESPONSE = 222,
	//ROOM_CHAT_NOTIFY = 223,

	PACKET_CHATNAME_REQ = 1001,
};

class Client_Packet
{
};