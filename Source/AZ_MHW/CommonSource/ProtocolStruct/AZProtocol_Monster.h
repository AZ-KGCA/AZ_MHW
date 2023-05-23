#pragma once
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZProtocol_Monster.generated.h"

// S->C Packet to adjust the basic transform of a monster
USTRUCT(BlueprintType)
struct FCG_MONSTER_TRANSFORM_CMD
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 monster_serial;
	UPROPERTY(VisibleAnywhere) FVector location;
	UPROPERTY(VisibleAnywhere) FRotator rotation;
	UPROPERTY(VisibleAnywhere) bool is_forced;

	FCG_MONSTER_TRANSFORM_CMD() : monster_serial(-1), location(FVector::ZeroVector), rotation(FRotator::ZeroRotator), is_forced(false) {};
};

// S->C Packet to initialise the states (part break, wound, sever) of body parts
USTRUCT(BlueprintType)
struct FCG_MONSTER_BODY_STATE_CMD
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere) int32 monster_serial;
	UPROPERTY(VisibleAnywhere) TMap<EMonsterBodyPart, FBossBodyPartDebuffState> body_state;
	
	FCG_MONSTER_BODY_STATE_CMD() : monster_serial(-1) {};
};

// S->C Packet to indicate enter combat
USTRUCT(BlueprintType)
struct FCG_MONSTER_ENTER_COMBAT_CMD
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere) int32 monster_serial;
	
	FCG_MONSTER_ENTER_COMBAT_CMD() : monster_serial(-1) {};
};

// S->C Packet to inform the start of new action of a monster
USTRUCT(BlueprintType)
struct FCG_MONSTER_ACTION_START_CMD
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere) int32 monster_serial;
	UPROPERTY(VisibleAnywhere) FMonsterActionStateInfo state_info;
	UPROPERTY(VisibleAnywhere) float start_position;

	FCG_MONSTER_ACTION_START_CMD() : monster_serial(-1), state_info(FMonsterActionStateInfo()), start_position(0.0f) {};
};

// S->C Packet to inform the state change (part break, wound, sever) of body parts
USTRUCT(BlueprintType)
struct FCG_MONSTER_PART_CHANGE_CMD
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 monster_serial;
	UPROPERTY(VisibleAnywhere) EMonsterBodyPart body_part;
	UPROPERTY(VisibleAnywhere) EMonsterBodyPartChangeType change_type;

	FCG_MONSTER_PART_CHANGE_CMD() : monster_serial(-1), body_part(EMonsterBodyPart::Default), change_type(EMonsterBodyPartChangeType::Wound) {};
};

// S->C Packet to inform the success of player's attack to a monster
USTRUCT(BlueprintType)
struct FCG_PLAYER_ATTACK_HIT_CMD
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere) int32 player_serial;
	UPROPERTY(VisibleAnywhere) int32 monster_serial;
	UPROPERTY(VisibleAnywhere) FVector hit_position;
	UPROPERTY(VisibleAnywhere) int32 damage_amount;

	FCG_PLAYER_ATTACK_HIT_CMD() : player_serial(-1), monster_serial(-1), hit_position(FVector::ZeroVector), damage_amount(0) {};
};
