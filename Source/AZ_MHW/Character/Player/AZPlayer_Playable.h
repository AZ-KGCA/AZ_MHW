// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "Character/Player/AZPlayer.h"
#include "AZPlayer_Playable.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration
class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;
#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion
/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayer_Playable : public AAZPlayer
{
	GENERATED_BODY()

public:
	AAZPlayer_Playable();
	
#pragma region Inherited function
protected:
	/** */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
#pragma endregion
public:
	/** 플레이어블 카메라
	 * 나중에 카메라 매니저를 따로 만들어서 관리예정
	 */
	//UPROPERTY(VisibleDefaultsOnly, Category=Camera)
	//bCanControlCamera = true;
	UPROPERTY(VisibleDefaultsOnly, Category=Camera)
	USpringArmComponent* SpringArmComp;//CameraControll
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	UCameraComponent* CameraComp;//Camera

	void ActionLook(const FInputActionValue& Value);
};