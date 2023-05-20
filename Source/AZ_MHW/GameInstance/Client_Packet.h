#pragma once
#include "Define.h"

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
	IN_GAME_BUY_REQUEST = 502,
	IN_GAME_EQUIP_CHANGE_REQUEST = 503,
	
	// Server -> Client
	IN_GAME_REMOTABLE_CREATE_REQUEST = 10001,//원격 생성요청
	IN_GAME_REMOTABLE_DESTROY_REQUEST = 10002,//원격 제거요청
	IN_GAME_REMOTABLE_EQUIP_CHANGE_REQUEST = 10003,//원격 장비변경
	IN_GAME_REMOTABLE_INPUT_REQUEST = 10004,//원격 인풋요청
	IN_GAME_REMOTABLE_HIT_REQUEST = 10005,//원격 피격요청
	//IN_GAME_UPDATE_MERCHANT_REQUEST = 10006,//상점 갱신요청
	//IN_GAME_UPDATE_GATHERING_REQUEST = 10007,//필드 채집물 갱신요청
	
	
	PACKET_CHATNAME_REQ = 1001,
};




class Client_Packet
{
};



struct InputPacket
{
	unsigned short packet_length;
	unsigned short packet_id;
	
	FVector current_position;
	FRotator current_direction;
	
	FRotator input_direction;
	int32 input_data;

	InputPacket()
	{
		packet_id = static_cast<int>(CLIENT_PACKET_ID::IN_GAME_INPUT_REQUEST);
		packet_length = sizeof(InputPacket);
		input_data = 0;
	}
};

struct StatePacket
{
	unsigned short packet_length;
	unsigned short packet_id;
	
};

struct RemotablePacket
{
	unsigned short packet_length;
	unsigned short packet_id;
	
	FVector current_position;
	FRotator current_direction;
	
	FRotator input_direction;
	int32 input_data;
};