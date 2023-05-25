#pragma once

//캐릭터 생성패킷?
struct CREATE_PLAYER_CHARACTER_PACKET : public PACKET_HEADER
{
	int32 guid;
	int32 character_index;
	
	CREATE_PLAYER_CHARACTER_PACKET()
	{
		packet_length = sizeof(CREATE_PLAYER_CHARACTER_PACKET);
		guid = -1;
	}
};

struct REMOVE_PLAYER_CHARACTER_PACKET : public PACKET_HEADER
{
	int32 guid;
	int32 character_index;

	REMOVE_PLAYER_CHARACTER_PACKET()
	{
		packet_length = sizeof(REMOVE_PLAYER_CHARACTER_PACKET);
		guid = -1;
	}
	
};

struct ACTION_PLAYER_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	FVector current_position;
	float current_direction;
	float input_direction;
	int32 input_data;

	ACTION_PLAYER_PACKET()
	{
		packet_length = sizeof(ACTION_PLAYER_PACKET);
		guid = -1;
		
		current_direction = 0.f;
		input_direction = 0.f;
		input_data = 0;
	}
};

struct UPDATE_PLAYER_STATE_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	int32 state_value;
	int32 value;

	UPDATE_PLAYER_STATE_PACKET()
	{
		packet_length = sizeof(UPDATE_PLAYER_STATE_PACKET);
		guid = -1;
		
	}
};

struct EQUIPMENT_PLAYER_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	int32 item_head_id;
	int32 item_hair_id;
	int32 item_body_id;
	int32 item_leg_id;
	int32 item_arm_id;
	int32 item_waist_id;
	
	int32 item_weapon_id;
	int32 item_consume_id;
	int32 item_ammo_id;

	EQUIPMENT_PLAYER_PACKET()
	{
		packet_length = sizeof(EQUIPMENT_PLAYER_PACKET);
		guid = -1;
		
	}
};

struct INVENTORY_PLAYER_PACKET : public PACKET_HEADER
{
	int32 guid;
	
	int32 item_id;
	int32 item_count;
	int32 item_pos;
	//0 == warehouse
	//1 == inventory
	//2 == pocket
	
	INVENTORY_PLAYER_PACKET()
	{
		packet_length = sizeof(INVENTORY_PLAYER_PACKET);
		guid = -1;
		
		item_id = -1;
		item_count = -1;
		item_pos = -1;
	}
	
};
