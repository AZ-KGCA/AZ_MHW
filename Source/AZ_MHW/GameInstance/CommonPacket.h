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

	CS_LOGIN_SIGNIN_REQ = 1000,
	CS_LOGIN_SIGNIN_RES = 1001,
	CS_LOGIN_SIGNUP_REQ = 1002,
	CS_LOGIN_SIGNUP_RES = 1003,

	CS_CHAT_MSG_CMD = 1010,
	SC_CHAT_MSG_CMD = 1011,

	CS_ITEM_TOTAL_INFO_REQ = 1100,
	CS_ITEM_TOTAL_INFO_RES = 1101,
	CS_ITEM_UNEQUIP_REQ = 1102,
	CS_ITEM_UNEQUIP_RES = 1103,
	CS_ITEM_EQUIP_REQ = 1104,
	CS_ITEM_EQUIP_RES = 1105,
	CS_ITEM_MOVE_REQ = 1106,
	CS_ITEM_MOVE_RES = 1107,
	CS_ITEM_CREATE_REQ = 1108,
	CS_ITEM_CREATE_RES = 1109,
	CS_ITEM_USE_REQ = 1110,
	CS_ITEM_USE_RES = 1111,
	SC_ITEM_INFO_CMD = 1112

	// Monster------------------------
	// Server -> Client
	SC_MONSTER_SPAWN_CMD = 7000,
	SC_MONSTER_TRANSFORM_CMD = 7001,
	SC_MONSTER_BODY_STATE_CMD = 7002,
	SC_MONSTER_ACTION_START_CMD = 7100,
	SC_MONSTER_ENTER_COMBAT_CMD = 7200,
	SC_MONSTER_PART_CHANGE_CMD = 7201,
	SC_MONSTER_HIT_CMD = 7202,
	SC_MONSTER_DIE_CMD = 7203,
	
	// End of Monster-----------------
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
	INT32 success = 0;
};

#include "AZ_MHW/SocketHolder/Item/ItemPacket.h"
#include "AZ_MHW/SocketHolder/Chat/ChatPacket.h"
#include "AZ_MHW/SocketHolder/Login/LoginPacket.h"
#include "AZ_MHW/SocketHolder/Map/MapPacket.h"
#include "AZ_MHW/SocketHolder/Monster/MonsterPacket.h"
#pragma pack(pop) //위에 설정된 패킹설정이 사라짐
