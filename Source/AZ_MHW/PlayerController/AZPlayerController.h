// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <GameFramework/PlayerController.h>
#include "AZPlayerController.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration

#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion
/**
 * 모든 PlayerController Base
 * 
 * Menu(Login)
 * CreateCharacter()
 * Lobby(Town)
 * InGame(Wild)
 */
UCLASS()
class AZ_MHW_API AAZPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAZPlayerController();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void OnPossess(APawn* in_pawn) override;
	/** */
	virtual void SetupInputComponent() override;
#pragma endregion
public:
	//UPROPERTY(BlueprintReadOnly) AAZCameraManager* CameraMgr;
	//UPROPERTY(BlueprintReadOnly) AAZHUDManager* HUDMgr;
	//UPROPERTY(BlueprintReadOnly) AAZPlayerState* State;
};
