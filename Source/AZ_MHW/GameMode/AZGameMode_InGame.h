// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZGameMode_InGame.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZGameMode_InGame : public AAZGameMode
{
	GENERATED_BODY()
public:
	AAZGameMode_InGame();

	virtual void PostLogin(APlayerController* NewPlayer) override;
};
