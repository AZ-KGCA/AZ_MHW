// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayerController_InGame.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/PlayerState/AZPlayerState.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/Character/Player/AZPlayer_Remotable.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

//#include <Components/SkinnedMeshComponent.h>

AAZPlayerController_InGame::AAZPlayerController_InGame()
{
	//PrimaryActorTick.bStartWithTickEnabled = true;

}


void AAZPlayerController_InGame::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);

	playable_player_ = Cast<AAZPlayer_Playable>(pawn);
	playable_player_state_ = Cast<AAZPlayerState>(playable_player_->GetPlayerState());
}

void AAZPlayerController_InGame::AddRemotePlayer(int32 guid, AAZPlayerState* other_player_state)
{
	//id GetWorld()->SpawnActor<AAZPlayer_Remotable>();
	
}




void AAZPlayerController_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	AZGameInstance->input_mgr_->ClearInputMappingContext();
	//AZGameInstance->input_mgr->AddInputMappingContext(TEXT("UI"), GetLocalPlayer());
	AZGameInstance->input_mgr_->AddInputMappingContext(TEXT("InGame"));
	//Camera Rotate Action
	
	//TODO: 임시 무기 종류시, 종류에 따라 매핑하기
	AZGameInstance->input_mgr_->AddInputMappingContext(TEXT("MeleeWeapons"));
	//AZGameInstance->input_mgr->AddInputMappingContext(TEXT("RangedWeapons"), GetLocalPlayer());
	
	//AddInputContextMapping(FName(TEXT("Sequence")));
	//AddInputContextMapping(FName(TEXT("Common")));
	
	
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UAZInputMgr* input_mgr = AZGameInstance->input_mgr_;
		//W
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveForward"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveForward_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveForward"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveForward_End);
		//S
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveBack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveBack_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveBack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveBack_End);
		//A
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveLeft"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveLeft_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveLeft"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveLeft_End);
		//D
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveRight"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveRight_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveRight"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveRight_End);
		//WASD Direction
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("InputDirection"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionInputDirection);
		
		//MLB
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("NormalAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionNormalAttack_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("NormalAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionNormalAttack_End);
		//MRB
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("SpecialAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionSpecialAttack_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("SpecialAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionSpecialAttack_End);

		//LCtrl
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UniqueAction"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUniqueAction_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UniqueAction"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUniqueAction_End);
		//LShift
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("DashHold"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashHold_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("DashHold"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashHold_End);
		
		//Space
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("Dodge"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDodge_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("Dodge"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDodge_End);
		//B
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("DashOnce"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashOnce_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("DashOnce"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashOnce_End);
		//E
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UseItem"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUseItem_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UseItem"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUseItem_End);
	}
}

void AAZPlayerController_InGame::ActionInputDirection()
{
	//키입력방향
	int movement_x = (static_cast<int>(playable_player_state_->action_state_.bit_move_back) - static_cast<int>(playable_player_state_->action_state_.bit_move_forward));
	int movement_y = (static_cast<int>(playable_player_state_->action_state_.bit_move_left) - static_cast<int>(playable_player_state_->action_state_.bit_move_right));
	FVector2D movement_vector(movement_x, movement_y);

	//현재 카메라방향
	FRotator control_rotation = GetControlRotation();
	FRotator yaw_rotation(0, control_rotation.Yaw, 0);
	FVector forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X);
	FVector right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y);
	
	//카메라 방향에 대한 입력방향(플레이어가 인지하는 방향)
	FVector movement_direction = (right_direction * movement_vector.Y + forward_direction * movement_vector.X).GetSafeNormal();
	playable_player_state_->action_state_.input_direction = -movement_direction;
}

void AAZPlayerController_InGame::ActionMoveForward_Start()
{
	playable_player_state_->action_state_.bit_move_forward = true;
}

void AAZPlayerController_InGame::ActionMoveForward_End()
{
	playable_player_state_->action_state_.bit_move_forward = false;
}

void AAZPlayerController_InGame::ActionMoveLeft_Start()
{
	playable_player_state_->action_state_.bit_move_left = true;
}

void AAZPlayerController_InGame::ActionMoveLeft_End()
{
	playable_player_state_->action_state_.bit_move_left = false;
}

void AAZPlayerController_InGame::ActionMoveRight_Start()
{
	playable_player_state_->action_state_.bit_move_right = true;
}

void AAZPlayerController_InGame::ActionMoveRight_End()
{
	playable_player_state_->action_state_.bit_move_right = false;
}

void AAZPlayerController_InGame::ActionMoveBack_Start()
{
	playable_player_state_->action_state_.bit_move_back = true;
}

void AAZPlayerController_InGame::ActionMoveBack_End()
{
	playable_player_state_->action_state_.bit_move_back = false;
}

void AAZPlayerController_InGame::ActionUniqueAction_Start()
{
	playable_player_state_->action_state_.bit_unique_action = true;
}


void AAZPlayerController_InGame::ActionUniqueAction_End()
{
	playable_player_state_->action_state_.bit_unique_action = false;
}

void AAZPlayerController_InGame::ActionNormalAttack_Start()
{
	playable_player_state_->action_state_.bit_normal_action = true;
}

void AAZPlayerController_InGame::ActionNormalAttack_End()
{
	playable_player_state_->action_state_.bit_normal_action = false;
}

void AAZPlayerController_InGame::ActionSpecialAttack_Start()
{
	playable_player_state_->action_state_.bit_special_action = true;
}

void AAZPlayerController_InGame::ActionSpecialAttack_End()
{
	playable_player_state_->action_state_.bit_special_action = false;
}

void AAZPlayerController_InGame::ActionDashHold_Start()
{
	playable_player_state_->action_state_.bit_dash_action = true;
}

void AAZPlayerController_InGame::ActionDashHold_End()
{
	playable_player_state_->action_state_.bit_dash_action = false;
}

void AAZPlayerController_InGame::ActionDodge_Start()
{
	playable_player_state_->action_state_.bit_evade_action = true;
}

void AAZPlayerController_InGame::ActionDodge_End()
{
	playable_player_state_->action_state_.bit_evade_action = false;
}

//delegate만들어서 아이템사용로직 붙힐 수 있게 하기
void AAZPlayerController_InGame::ActionUseItem_Start()
{
	playable_player_state_->action_state_.bit_use_item = true;
}

void AAZPlayerController_InGame::ActionUseItem_End()
{
	playable_player_state_->action_state_.bit_use_item = false;
}

void AAZPlayerController_InGame::ActionInteract_Start()
{
	playable_player_state_->action_state_.bit_interaction = true;
}

void AAZPlayerController_InGame::ActionInteract_End()
{
	playable_player_state_->action_state_.bit_interaction = false;
}

void AAZPlayerController_InGame::ActionDashOnce_Start()
{
	//PlayablePlayerState->ActionState.bDashOnce = true;
}

void AAZPlayerController_InGame::ActionDashOnce_End()
{
	//PlayablePlayerState->ActionState.bDashOnce = false;
}
