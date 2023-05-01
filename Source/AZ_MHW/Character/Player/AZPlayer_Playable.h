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

DECLARE_MULTICAST_DELEGATE(FOnUseItemDelegate);

class USpringArmComponent;
class UCameraComponent;
struct FInputActionValue;

class AAZPlayerController_InGame;
class AAZPlayerState;
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
	/** */
	virtual void PossessedBy(AController* NewController) override;
	/** */
	virtual void Tick(float DeltaSeconds) override;
#pragma endregion
public:
	UPROPERTY() AAZPlayerController_InGame* PlayablePlayerController;
	UPROPERTY() AAZPlayerState* PlayablePlayerState;
	
	/** 플레이어블 카메라용도 컴포넌트
	 * 나중에 카메라 매니저를 따로 만들어서 관리해야 시네마틱카메라할때 편함.
	 */
	UPROPERTY(VisibleDefaultsOnly, Category=Camera)
	USpringArmComponent* SpringArmComp;//CameraControll
	UPROPERTY(VisibleDefaultsOnly, Category = Camera)
	UCameraComponent* CameraComp;//Camera

	
	UFUNCTION() void AnimNotify_OnUseItem();

public:
	FOnUseItemDelegate OnUseItem;
	
	void ActionLook(const FInputActionValue& Value);

	//환경 감지하기?
	//박스 이용? 어떻게해
	
};