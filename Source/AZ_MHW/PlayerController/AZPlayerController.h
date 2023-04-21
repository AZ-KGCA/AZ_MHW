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
 * PlayerControllerBase Class
 * 
 * Menu(Login)
 * CreateCharacter
 * Lobby(Town)
 * InGame(Wild)
 * Sequence
 * 
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
	virtual void OnPossess(APawn* InPawn) override;
	/** */
	virtual void SetupInputComponent() override;
#pragma endregion
public:
	/** */
	void AddInputMappingContext(FName IMCName) const; 
	/** */ 
	void RemoveInputMappingContext(FName IMCName) const;
	/** */
	void ClearInputMappingContext() const;
	
	//UPROPERTY(BlueprintReadOnly) AAZCameraManager* CameraMgr;
	//UPROPERTY(BlueprintReadOnly) AAZHUDManager* HUDMgr;
	//UPROPERTY(BlueprintReadOnly) AAZPlayerState* State;
};
