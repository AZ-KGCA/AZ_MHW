#pragma once
#include "AZ_MHW/CommonSource/AZStruct.h"

// Monster base packet
struct FMonsterBasePacket : PACKET_HEADER
{
	int32 object_serial;

	FMonsterBasePacket() : object_serial(-1) {};
	FMonsterBasePacket(int32 serial) : object_serial(serial) {};
};

// S->C Packet to spawn a monster at a location
struct SC_MONSTER_SPAWN_CMD : FMonsterBasePacket
{
	int32 monster_id;
	FVector location;
	FRotator rotation;

	SC_MONSTER_SPAWN_CMD() :
		monster_id(-1), location(FVector::ZeroVector), rotation(FRotator::ZeroRotator) {};
	SC_MONSTER_SPAWN_CMD(int32 serial) :
		monster_id(-1), location(FVector::ZeroVector), rotation(FRotator::ZeroRotator) {};
};

// S->C Packet to adjust the basic transform of a monster
// is_forced: true when used for initialisation
struct SC_MONSTER_TRANSFORM_CMD : FMonsterBasePacket
{
	FVector location;
	FRotator rotation;
	bool is_forced;

	SC_MONSTER_TRANSFORM_CMD() : location(FVector::ZeroVector),
		rotation(FRotator::ZeroRotator), is_forced(true) {};
	SC_MONSTER_TRANSFORM_CMD(int32 serial) : location(FVector::ZeroVector),
		rotation(FRotator::ZeroRotator), is_forced(true) {};
};

// S->C Packet to initialise the states (part break, wound, sever) of body parts
// body_states map size is fixed to 
struct SC_MONSTER_BODY_STATE_CMD : FMonsterBasePacket
{
	FBossBodyPartDebuffState head_state;
	FBossBodyPartDebuffState back_state;
	FBossBodyPartDebuffState left_wing_state;
	FBossBodyPartDebuffState right_wing_state;
	FBossBodyPartDebuffState tail_state;

	SC_MONSTER_BODY_STATE_CMD(){};
	SC_MONSTER_BODY_STATE_CMD(int32 serial){};
};

// S->C Packet to indicate enter combat
struct SC_MONSTER_ENTER_COMBAT_CMD : FMonsterBasePacket
{
	SC_MONSTER_ENTER_COMBAT_CMD(){};
	SC_MONSTER_ENTER_COMBAT_CMD(int32 serial){};
};

// S->C Packet to inform the start of new action of a monster
struct SC_MONSTER_ACTION_START_CMD : FMonsterBasePacket
{
	MonsterActionStateInfo state_info;
	float start_position;

	SC_MONSTER_ACTION_START_CMD() :
		state_info(FMonsterActionStateInfo()), start_position(0.0f) {};
	SC_MONSTER_ACTION_START_CMD(int32 serial) :
		state_info(FMonsterActionStateInfo()), start_position(0.0f) {};
};

// S->C Packet to inform the state change (part break, wound, sever) of body parts
struct SC_MONSTER_PART_CHANGE_CMD : FMonsterBasePacket
{
	EMonsterBodyPart body_part;
	EMonsterBodyPartChangeType change_type;

	SC_MONSTER_PART_CHANGE_CMD() :
		body_part(EMonsterBodyPart::Default), change_type(EMonsterBodyPartChangeType::Wound) {};
	SC_MONSTER_PART_CHANGE_CMD(int32 serial) :
		body_part(EMonsterBodyPart::Default), change_type(EMonsterBodyPartChangeType::Wound) {};
};

// S->C Packet to inform the success of player's attack to a monster
struct SC_MONSTER_HIT_CMD : FMonsterBasePacket
{
	FVector hit_position;
	int32 damage_amount;

	SC_MONSTER_HIT_CMD() :
		hit_position(FVector::ZeroVector), damage_amount(0) {};
	SC_MONSTER_HIT_CMD(int32 serial) :
		hit_position(FVector::ZeroVector), damage_amount(0) {};
};

struct SC_MONSTER_DIE_CMD : FMonsterBasePacket
{
	SC_MONSTER_DIE_CMD(){};
	SC_MONSTER_DIE_CMD(int32 serial){};
};