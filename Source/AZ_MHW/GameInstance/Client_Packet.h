#pragma once
#include "Define.h"
#include <AZ_MHW/CommonSource/MagicEnum/magic_enum.hpp>


struct BasePacket
{
	unsigned short packet_length;
	unsigned short packet_id;
};

struct Login_Send_Packet : public BasePacket
{
	//bool packet_type;
	char user_id[33];
	char user_pw[33];
};

struct SetMoveInfo : public BasePacket
{
	FVector fvector_;
	FRotator frotator_;
};

struct header_check_packet : public BasePacket
{
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

	IN_GAME_INPUT_REQUEST = 501,
	
	PACKET_CHATNAME_REQ = 1001,
	
};

class Client_Packet
{
};

struct Input_Packet
{
	unsigned short packet_length;
	unsigned short packet_id;
	
	FVector current_position;
	FRotator current_direction;
	
	FRotator input_direction;
	int32 input_data;

	Input_Packet()
	{
		packet_id = static_cast<int>(CLIENT_PACKET_ID::IN_GAME_INPUT_REQUEST);
		packet_length = sizeof(Input_Packet);
		input_data = 0;
	}
};

template<>
struct magic_enum::customize::enum_range<CLIENT_PACKET_ID>
{
	static constexpr int min = 1;
	static constexpr int max = 10000;
};