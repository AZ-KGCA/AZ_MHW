// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/AZPlayer_Playable.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include "GameInstance/AZGameInstance.h"
#include "Manager/AZInputMgr.h"
#include "AnimInstance/AZAnimInstance_Player.h"

#include "PlayerState/AZPlayerState.h"
//#include <InputActionValue.h>

AAZPlayer_Playable::AAZPlayer_Playable()
{	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetupAttachment(SpringArmComp);

	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp->bUsePawnControlRotation = false;
}

//TODO: CameraManager 만들어서 플레이어 캐릭터와 입력처리 완전히 분리
void AAZPlayer_Playable::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(DEREF_IAMAP("Look"), ETriggerEvent::Triggered, this, &AAZPlayer_Playable::ActionLook);
		//...
	}
}


void AAZPlayer_Playable::ActionLook(const FInputActionValue& Value)
{
	//변경하지 않는 것에 대한 상수화
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

/*
*void AAZPlayer_Playable::ActionWASD(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement
		//g회전
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}
*/