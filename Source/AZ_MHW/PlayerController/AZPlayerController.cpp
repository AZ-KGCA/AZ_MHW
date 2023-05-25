// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController.h"

#include "EnhancedInputComponent.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZInputMgr.h"


AAZPlayerController::AAZPlayerController()
{
	
}

void AAZPlayerController::BeginPlay()
{
	Super::BeginPlay();
	//?왜 여기선 GetWorld를 못하는가
	////액터가 완전히 안만들어져서 World에 대한 무언가가 없다?
	//게임 모드 로드중 월드 생성도중에 생성되는 액터라서 GetWorld가 안되는가?
	//GEngine->GetWorld()도 불가능.
}

void AAZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	
	game_instance_->input_mgr_->SetupDefaultBindAction(InputComponent, GetLocalPlayer());
	game_instance_->input_mgr_->AddInputMappingContext(TEXT("UI"));
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UAZInputMgr* input_mgr = game_instance_->input_mgr_;
		//ESC
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("ChangeControlMode"), ETriggerEvent::Triggered, this, &AAZPlayerController:: ActionChangeControlMode);
	}
}


void AAZPlayerController::OnPossess(APawn* in_pawn)
{
	Super::OnPossess(in_pawn);
	
}

void AAZPlayerController::BeginDestroy()
{
	Super::BeginDestroy();
}


void AAZPlayerController::ActionChangeControlMode()
{
	//TODO: UIMode <-> InGameMode
	FViewport* viewport = GetLocalPlayer()->ViewportClient->Viewport;
	bool is_mouse_capture = viewport->HasMouseCapture();
	if(is_mouse_capture)
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		
		viewport->CaptureMouse(false);
	}
	else
	{
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;

		viewport->CaptureMouse(true);
	}
}



