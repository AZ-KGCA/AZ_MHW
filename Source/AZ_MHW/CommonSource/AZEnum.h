// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Math/NumericLimits.h>
#include "AZEnum.generated.h"

UENUM(BlueprintType)
enum class EAutoMove : uint8
{
	None
};

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