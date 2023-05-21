#pragma once
#include "Define.h"

struct Login_Send_Packet
{
	unsigned short packet_length;
	unsigned short packet_id;
	unsigned short clinet_id;
	//bool packet_type;
	char user_id[33];
	char user_pw[33];
};

struct SetMoveInfo
{
	unsigned short packet_length;
	unsigned short packet_id;
	unsigned short clinet_id;
	FVector fvector_;
	FRotator frotator_;
};

struct header_check_packet
{
	unsigned short packet_length;
	unsigned short packet_id;
	unsigned short clinet_id;

	char user_id[33];
	char user_pw[33];
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

	IN_GAME_REQUEST = 401,
	IN_GAME_SUCCESS = 402,
	IN_GAME_FAIL = 403,

	IN_GAME_MOVE_START = 501,
	IN_GAME_MOVE_END = 502,
	//IN_GAME_SUCCESS = 402,
	//IN_GAME_FAIL = 403,

	MONSTER_REQUEST = 601, // 몬스터 생성 요청 (클라 -> 서버로 전달)
	MONSTER_RESPONSE = 602, // 몬스터 생성 응답 (서버 -> 클라로 전달) // 클라에서는 응답 할 일이 없기 때문에 필요없지만 일단 넣어둠 ..


	PACKET_CHATNAME_REQ = 1001,
};

class Client_Packet
{
};