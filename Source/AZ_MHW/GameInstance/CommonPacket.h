#pragma once
#include "Define.h"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum.hpp"

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

	PACKET_CHATNAME_REQ = 1001,
};

template<>
struct magic_enum::customize::enum_range<PACKET_ID>
{
	static constexpr int min = 1;
	static constexpr int max = 10000;
};

#pragma pack(push,1)
struct PACKET_HEADER
{
	UINT16 packet_length;
	UINT16 packet_id;
};
struct LOGIN_REQUEST_PACKET : public PACKET_HEADER
{
	char user_id[33];
	char user_pw[33];
};

struct LOGIN_RESPONSE_PACKET : public PACKET_HEADER
{
	UINT16 result;
};

struct INPUT_PACKET : public PACKET_HEADER
{
	FVector current_position;
	FRotator current_direction;

	FRotator input_direction;
	int32 input_data;

	INPUT_PACKET()
	{
		packet_id = static_cast<int>(PACKET_ID::IN_GAME_INPUT_REQUEST);
		packet_length = sizeof(INPUT_PACKET);
		input_data = 0;
	}
};
#pragma pack(pop) //위에 설정된 패킹설정이 사라짐