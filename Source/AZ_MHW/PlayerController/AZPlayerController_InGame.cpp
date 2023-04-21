// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/AZPlayerController_InGame.h"
//#include "Components/SkinnedMeshComponent.h"
#include "Manager/AZInputMgr.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include "GameInstance/AZGameInstance.h"
#include "PlayerState/AZPlayerState.h"

#include "Character/Player/AZPlayer_Playable.h"
#include "Character/Player/AZPlayer_Remotable.h"

AAZPlayerController_InGame::AAZPlayerController_InGame()
{
	//PrimaryActorTick.bStartWithTickEnabled = true;

}

void AAZPlayerController_InGame::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);

	PlayableCharacter = Cast<AAZPlayer_Playable>(aPawn);
	//TODO: Controller 생성마다 State저장 ->AAZController로 옮기기
	PlayablePlayerState = Cast<AAZPlayerState>(PlayableCharacter->GetPlayerState());
}

void AAZPlayerController_InGame::CloneRemotePlayer(int32 Guid, AAZPlayerState* OtherPlayerState)
{
	
}




void AAZPlayerController_InGame::SetupInputComponent()
{
	ClearInputMappingContext();
	AddInputMappingContext(FName(TEXT("UI")));
	//AddInputContextMapping(FName(TEXT("Sequence")));
	AddInputMappingContext(FName(TEXT("InGame")));
	//AddInputContextMapping(FName(TEXT("Common")));
	
	//TODO: 임시 무기발도시, 종류에 따라 매핑하기
	AddInputMappingContext(FName(TEXT("Melee")));
	//AddInputMappingContext(FName(TEXT("Ranged")));
	//AddInputMappingContext(FName(TEXT("Sheathe")));
	//AddInputMappingContext(FName(TEXT("ClutchClaw")));
	//AddInputMappingContext(FName(TEXT("Vehicle")));
	
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		//W
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveForward"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveForward_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveForward"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveForward_End);
		//S
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveBack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveBack_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveBack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveBack_End);
		//A
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveLeft"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveLeft_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveLeft"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveLeft_End);
		//D
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveRight"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveRight_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("MoveRight"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveRight_End);
		//WASD Direction
		EnhancedInputComponent->BindAction(DEREF_IAMAP("Direction"),ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDirection);
		//MLB
		EnhancedInputComponent->BindAction(DEREF_IAMAP("NormalAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionNormalAttack_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("NormalAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionNormalAttack_End);
		//MRB
		EnhancedInputComponent->BindAction(DEREF_IAMAP("SpecialAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionSpecialAttack_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("SpecialAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionSpecialAttack_End);

		//LCtrl
		EnhancedInputComponent->BindAction(DEREF_IAMAP("UniqueAction"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUniqueAction_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("UniqueAction"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUniqueAction_End);
		//LShift
		EnhancedInputComponent->BindAction(DEREF_IAMAP("DashHold"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashHold_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("DashHold"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashHold_End);
		
		//Space
		EnhancedInputComponent->BindAction(DEREF_IAMAP("Dodge"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDodge_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("Dodge"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDodge_End);
		//B
		EnhancedInputComponent->BindAction(DEREF_IAMAP("DashOnce"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashOnce_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("DashOnce"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashOnce_End);
		//E
		EnhancedInputComponent->BindAction(DEREF_IAMAP("UseItem"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUseItem_Start);
		EnhancedInputComponent->BindAction(DEREF_IAMAP("UseItem"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUseItem_End);
	}
}

void AAZPlayerController_InGame::ActionDirection()
{
	const int MovementX = (static_cast<int>(PlayablePlayerState->ActionState.bMoveBack) - static_cast<int>(PlayablePlayerState->ActionState.bMoveForward));
	const int MovementY = (static_cast<int>(PlayablePlayerState->ActionState.bMoveLeft) - static_cast<int>(PlayablePlayerState->ActionState.bMoveRight));
	const FVector2D MovementVector(MovementX, MovementY);
	
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	const FVector MovementDirection = (RightDirection * MovementVector.Y + ForwardDirection * MovementVector.X).GetSafeNormal();
	PlayablePlayerState->ActionState.Direction = -MovementDirection;
}

void AAZPlayerController_InGame::ActionMoveForward_Start()
{
	//누를 수 있거나, 이미 눌리고 있거나
	PlayablePlayerState->ActionState.bMoveForward = true;
}

void AAZPlayerController_InGame::ActionMoveForward_End()
{
	PlayablePlayerState->ActionState.bMoveForward = false;
}

void AAZPlayerController_InGame::ActionMoveLeft_Start()
{
	PlayablePlayerState->ActionState.bMoveLeft = true;
}

void AAZPlayerController_InGame::ActionMoveLeft_End()
{
		PlayablePlayerState->ActionState.bMoveLeft = false;
}

void AAZPlayerController_InGame::ActionMoveRight_Start()
{
	PlayablePlayerState->ActionState.bMoveRight = true;
}

void AAZPlayerController_InGame::ActionMoveRight_End()
{
	PlayablePlayerState->ActionState.bMoveRight = false;
}

void AAZPlayerController_InGame::ActionMoveBack_Start()
{
	PlayablePlayerState->ActionState.bMoveBack = true;
}

void AAZPlayerController_InGame::ActionMoveBack_End()
{
	PlayablePlayerState->ActionState.bMoveBack = false;
}

void AAZPlayerController_InGame::ActionUniqueAction_Start()
{
	PlayablePlayerState->ActionState.bUniqueWeaponAction = true;
}


void AAZPlayerController_InGame::ActionUniqueAction_End()
{
	PlayablePlayerState->ActionState.bUniqueWeaponAction = false;
}

void AAZPlayerController_InGame::ActionNormalAttack_Start()
{
	PlayablePlayerState->ActionState.bNormalAttack = true;
}

void AAZPlayerController_InGame::ActionNormalAttack_End()
{
	PlayablePlayerState->ActionState.bNormalAttack = false;
}

void AAZPlayerController_InGame::ActionSpecialAttack_Start()
{
	PlayablePlayerState->ActionState.bSpecialAttack = true;
}

void AAZPlayerController_InGame::ActionSpecialAttack_End()
{
	PlayablePlayerState->ActionState.bSpecialAttack = false;
}

void AAZPlayerController_InGame::ActionDashHold_Start()
{
	PlayablePlayerState->ActionState.bDashHold = true;
}

void AAZPlayerController_InGame::ActionDashHold_End()
{
	PlayablePlayerState->ActionState.bDashHold = false;
}

void AAZPlayerController_InGame::ActionDashOnce_Start()
{
	PlayablePlayerState->ActionState.bDashOnce = true;
}

void AAZPlayerController_InGame::ActionDashOnce_End()
{
	PlayablePlayerState->ActionState.bDashOnce = false;
}

void AAZPlayerController_InGame::ActionDodge_Start()
{
	PlayablePlayerState->ActionState.bDodge = true;
}

void AAZPlayerController_InGame::ActionDodge_End()
{
	PlayablePlayerState->ActionState.bDodge = false;
}

//delegate만들어서 아이템사용로직 붙힐 수 있게 하기
//OnUseItem
void AAZPlayerController_InGame::ActionUseItem_Start()
{
	PlayablePlayerState->ActionState.bUseItem = true;
}

void AAZPlayerController_InGame::ActionUseItem_End()
{
	PlayablePlayerState->ActionState.bUseItem = false;
}
