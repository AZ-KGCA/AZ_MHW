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
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	
}
void AAZPlayerController::OnPossess(APawn* in_pawn)
{
	Super::OnPossess(in_pawn);
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
}

void AAZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	game_instance_->input_mgr_->SetupDefaultBindAction(InputComponent, GetLocalPlayer());
	
}

