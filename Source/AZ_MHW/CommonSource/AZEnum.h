// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Math/NumericLimits.h>
#include "AZEnum.generated.h"

UENUM(BlueprintType)
enum class ESocketHolderType : uint8
{
	Login,
	Game,
	Max
};

UENUM(BlueprintType)
enum class EGameState : uint8
{
	None,
	GameDefault
};

UENUM(BlueprintType)
enum class EGameModeType : uint8
{
	None,
	Login,
	CharacterSelect,
	CharacterCreate,
	Lobby,
	InGame,
};

UENUM(BlueprintType)
enum class EGameModeFlag : uint8
{
	None,
	Launcher,
	Login,
	Lobby,
	InGame
};


// -------------------------------------------------
// Monster

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	None,
	NPC,
	Player,
	Monster,	
};

UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Locomotion,
	Action,
};

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Stop,
	Walk,
	Run,
	Fly,
};

UENUM(BlueprintType)
enum class EBossRank : uint8
{
	None,
	Low,
	Medium,
	Master,
};

UENUM(BlueprintType)
enum class EMonsterGoalSearchStyle : uint8
{
	Random,
	Chase,
	Escape,
};

UENUM(BlueprintType)
enum class EMonsterBehaviorType : uint8
{
	Neutral,
	Friendly,
	Hostile,
};

UENUM(BlueprintType)
enum class EMonsterActionMode : uint8
{
	Normal,
	Transition,
	Combat,
};

UENUM(BlueprintType)
enum class EMonsterActionPriority : uint8
{
	None,
	Locomotion,
	Action,
	Damaged,
	Death,
	Cinematic,
};

UENUM(BlueprintType)
enum class EAttackEffect : uint8
{
	None,
	KnockBack,
	RoarStagger,
	Stun,
};

UENUM(BlueprintType)
enum class EEffectDurationType : uint8
{
	Instant,
	ForDuration,
};

//TODO for now, no triggers are set for combat actions
UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMonsterActionTriggerType : uint8
{
	None			= 0,		
	Transition		= 1 << 0,	// used for transition actions only
	Damage			= 1 << 1,	// used for transition actions only
	Sight			= 1 << 2,	// used for transition actions only
	InRange			= 1 << 3,	// used for noncombat actions only
};
ENUM_CLASS_FLAGS(EMonsterActionTriggerType);

UENUM(BlueprintType, Meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EMonsterActionConditionType : uint8
{
	None		= 0,		
	Flying		= 1 << 0,	 // if false, must be on ground
	InRange		= 1 << 1,	
	Health		= 1 << 2,	
};
ENUM_CLASS_FLAGS(EMonsterActionConditionType);

// End of Monster
// -------------------------------------------------