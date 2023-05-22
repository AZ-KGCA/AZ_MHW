#pragma once
#include "Define.h"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum.hpp"

enum class PACKET_ID : UINT16
{
	MIN_PACKET_ID = 1,
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


	//SERVER INGAME BROADCAST REMOTABLE
	SC_BROADCAST_UPDATE_MERCHANT_CMD = 9000,//상점 정보 갱신 전파
	SC_BROADCAST_UPDATE_FIELD_CMD=9001,//필드 정보 갱신 전파
	SC_BROADCAST_CREATE_PLAYER_CMD=9002,//플레이어 생성 전파
	SC_BROADCAST_DESTROY_PLAYER_CMD=9003,//플레이어 제거 전파
	SC_BROADCAST_ACTION_PLAYER_CMD=9004,//플레이어 액션 전파
	SC_BROADCAST_EQUIP_PLAYER_CMD=9005,//플레이어 장비 전파
	SC_BROADCAST_UPDATE_PLAYER_CMD=9006,//플레이어 갱신 전파

	//CLIENT INGAME REQUEST
	CS_PLAYER_ORIGIN_CREATE_REQ=9101,//원본생성 요청 -> 원래는 응답받고 성공햇는지 체크해야하지만...
	CS_PLAYER_ORIGIN_DESTROY_REQ=9102,//원본제거 요청
	CS_PLAYER_ORIGIN_ACTION_REQ=9103,//원본액션 요청
	CS_PLAYER_ORIGIN_EQUIP_REQ=9104,//원본장비교체 요청
	CS_PLAYER_ORIGIN_BUY_REQ=9105,//구매 요청
	CS_PLAYER_ORIGIN_SELL_REQ=9106,//판매 요청
	
	SC_PLAYER_UPDATE_STATE_CMD=9200,//해당 플레이어 상태갱신 명령

	//CLIENT_LOGIN
	CS_PLAYER_GUID_REQ=9501,//아이디번호 요청
	SC_PLAYER_GUID_RES=9502,//아이디번호 응답

	//CLIENT_CHARACTER_DATA
	CS_PLAYER_CHARACTER_DATA_REQ=9503,//플레이어 캐릭터데이터 요청
	SC_PLAYER_CHARACTER_DATA_RES=9504,//플레이어 캐릭터데이터 응답
	CS_PLAYER_CHARACTER_CREATE_REQ=9505,//캐릭터창에서 캐릭터 생성 요청
	SC_PLAYER_CHARACTER_CREATE_RES=9506,//캐릭터창에서 캐릭터 생성 응답
	CS_PLAYER_CHARACTER_DESTROY_REQ=9507,//캐릭터창에서 캐릭터 제거 요청
	SC_PLAYER_CHARACTER_DESTROY_RES=9508,//캐릭터창에서 캐릭터 제거 응답
	
	MAX_PACKET_ID = 9999,
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

#include "SocketHolder/Character/CharacterPacket.h"

#pragma pack(pop) //위에 설정된 패킹설정이 사라짐