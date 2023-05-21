#pragma once
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZ_MHW/GameInstance/Client_Packet.h"

// Monster base packet
struct FMonsterBasePacket : BasePacket
{
	int32 monster_serial;

	FMonsterBasePacket() : monster_serial(-1) {};
	FMonsterBasePacket(int32 serial) : monster_serial(serial) {};
};

// S->C Packet to adjust the basic transform of a monster
struct FCG_MONSTER_TRANSFORM_CMD : FMonsterBasePacket
{
	FVector location;
	FRotator rotation;
	bool is_forced;

	FCG_MONSTER_TRANSFORM_CMD() : location(FVector::ZeroVector),
		rotation(FRotator::ZeroRotator), is_forced(false) {};
	FCG_MONSTER_TRANSFORM_CMD(int32 serial) : location(FVector::ZeroVector),
		rotation(FRotator::ZeroRotator), is_forced(false) {};
};

// S->C Packet to initialise the states (part break, wound, sever) of body parts
// body_states map size is fixed to 
struct FCG_MONSTER_BODY_STATE_CMD : FMonsterBasePacket
{
	FBossBodyPartDebuffState head_state;
	FBossBodyPartDebuffState back_state;
	FBossBodyPartDebuffState left_wing_state;
	FBossBodyPartDebuffState right_wing_state;
	FBossBodyPartDebuffState tail_state;
};

// S->C Packet to indicate enter combat
struct FCG_MONSTER_ENTER_COMBAT_CMD : FMonsterBasePacket
{
	FCG_MONSTER_ENTER_COMBAT_CMD(){};
	FCG_MONSTER_ENTER_COMBAT_CMD(int32 serial){};
};

// S->C Packet to inform the start of new action of a monster
struct FCG_MONSTER_ACTION_START_CMD : FMonsterBasePacket
{
	MonsterActionStateInfo state_info;
	float start_position;

	FCG_MONSTER_ACTION_START_CMD() :
		state_info(FMonsterActionStateInfo()), start_position(0.0f) {};
	FCG_MONSTER_ACTION_START_CMD(int32 serial) :
		state_info(FMonsterActionStateInfo()), start_position(0.0f) {};
};

// S->C Packet to inform the state change (part break, wound, sever) of body parts
struct FCG_MONSTER_PART_CHANGE_CMD : FMonsterBasePacket
{
	EMonsterBodyPart body_part;
	EMonsterBodyPartChangeType change_type;

	FCG_MONSTER_PART_CHANGE_CMD() :
		body_part(EMonsterBodyPart::Default), change_type(EMonsterBodyPartChangeType::Wound) {};
	FCG_MONSTER_PART_CHANGE_CMD(int32 serial) :
		body_part(EMonsterBodyPart::Default), change_type(EMonsterBodyPartChangeType::Wound) {};
};

// S->C Packet to inform the success of player's attack to a monster
struct FCG_PLAYER_ATTACK_HIT_CMD : FMonsterBasePacket
{
	int32 player_serial;
	FVector hit_position;
	int32 damage_amount;

	FCG_PLAYER_ATTACK_HIT_CMD() :
		player_serial(-1), hit_position(FVector::ZeroVector), damage_amount(0) {};
	FCG_PLAYER_ATTACK_HIT_CMD(int32 serial) :
		player_serial(-1), hit_position(FVector::ZeroVector), damage_amount(0) {};
};
