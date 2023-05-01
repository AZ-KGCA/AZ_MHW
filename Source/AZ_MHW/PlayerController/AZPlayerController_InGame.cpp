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
	PlayablePlayerState = Cast<AAZPlayerState>(PlayableCharacter->GetPlayerState());
}

void AAZPlayerController_InGame::CloneRemotePlayer(int32 Guid, AAZPlayerState* OtherPlayerState)
{
	//id GetWorld()->SpawnActor<AAZPlayer_Remotable>();
	
}




void AAZPlayerController_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	AZGameInstance->input_mgr->ClearInputMappingContext(GetLocalPlayer());
	//AZGameInstance->input_mgr->AddInputMappingContext(TEXT("UI"), GetLocalPlayer());
	AZGameInstance->input_mgr->AddInputMappingContext(TEXT("InGame"), GetLocalPlayer());
	//Camera Rotate Action
	
	//TODO: 임시 무기 종류시, 종류에 따라 매핑하기
	AZGameInstance->input_mgr->AddInputMappingContext(TEXT("MeleeWeapons"), GetLocalPlayer());
	//AZGameInstance->input_mgr->AddInputMappingContext(TEXT("RangedWeapons"), GetLocalPlayer());
	
	//AddInputContextMapping(FName(TEXT("Sequence")));
	//AddInputContextMapping(FName(TEXT("Common")));
	
	
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UAZInputMgr* InputMgr=AZGameInstance->input_mgr;
		//W
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveForward"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveForward_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveForward"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveForward_End);
		//S
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveBack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveBack_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveBack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveBack_End);
		//A
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveLeft"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveLeft_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveLeft"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveLeft_End);
		//D
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveRight"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveRight_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("MoveRight"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveRight_End);
		//WASD Direction
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("InputDirection"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionInputDirection);
		
		//MLB
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("NormalAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionNormalAttack_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("NormalAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionNormalAttack_End);
		//MRB
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("SpecialAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionSpecialAttack_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("SpecialAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionSpecialAttack_End);

		//LCtrl
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("UniqueAction"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUniqueAction_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("UniqueAction"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUniqueAction_End);
		//LShift
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("DashHold"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashHold_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("DashHold"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashHold_End);
		
		//Space
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("Dodge"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDodge_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("Dodge"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDodge_End);
		//B
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("DashOnce"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashOnce_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("DashOnce"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashOnce_End);
		//E
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("UseItem"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUseItem_Start);
		EnhancedInputComponent->BindAction(InputMgr->GetInputAction("UseItem"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUseItem_End);
	}
}

void AAZPlayerController_InGame::ActionInputDirection()
{
	//키입력
	const int MovementX = (static_cast<int>(PlayablePlayerState->ActionState.bMoveBack) - static_cast<int>(PlayablePlayerState->ActionState.bMoveForward));
	const int MovementY = (static_cast<int>(PlayablePlayerState->ActionState.bMoveLeft) - static_cast<int>(PlayablePlayerState->ActionState.bMoveRight));
	//키입력방향
	const FVector2D MovementVector(MovementX, MovementY);
	//현재 카메라방향
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	//카메라 방향에 대한 입력방향(플레이어가 인지하는 방향)
	const FVector MovementDirection = (RightDirection * MovementVector.Y + ForwardDirection * MovementVector.X).GetSafeNormal();
	PlayablePlayerState->ActionState.InputDirection = -MovementDirection;
}

void AAZPlayerController_InGame::ActionMoveForward_Start()
{
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

void AAZPlayerController_InGame::ActionDodge_Start()
{
	PlayablePlayerState->ActionState.bEvade = true;
}

void AAZPlayerController_InGame::ActionDodge_End()
{
	PlayablePlayerState->ActionState.bEvade = false;
}

//delegate만들어서 아이템사용로직 붙힐 수 있게 하기
void AAZPlayerController_InGame::ActionUseItem_Start()
{
	PlayablePlayerState->ActionState.bUseItem = true;
}

void AAZPlayerController_InGame::ActionUseItem_End()
{
	PlayablePlayerState->ActionState.bUseItem = false;
}

void AAZPlayerController_InGame::ActionDashOnce_Start()
{
	//PlayablePlayerState->ActionState.bDashOnce = true;
}

void AAZPlayerController_InGame::ActionDashOnce_End()
{
	//PlayablePlayerState->ActionState.bDashOnce = false;
}
