// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZInputMgr.h"


AAZPlayerController::AAZPlayerController()
{
	
}
void AAZPlayerController::BeginPlay()
{
	Super::BeginPlay();
}
void AAZPlayerController::OnPossess(APawn* in_pawn)
{
	Super::OnPossess(in_pawn);
	
}

void AAZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	AZGameInstance->input_mgr_->SetupDefaultBindAction(InputComponent, GetLocalPlayer());
	
}

