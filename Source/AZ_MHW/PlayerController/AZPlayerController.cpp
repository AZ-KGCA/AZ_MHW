// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayerController.h"
#include "GameInstance/AZGameInstance.h"
#include "Manager/AZInputMgr.h"//AZGameInstance.h
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>


AAZPlayerController::AAZPlayerController()
{
	
}
void AAZPlayerController::BeginPlay()
{
	Super::BeginPlay();
	PRINT_FUNCTION();
}
void AAZPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
}

void AAZPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	AZGameInstance->input_mgr->SetupDefaultBindAction(InputComponent);
	
	/*
	 *사용법
		//기존 인풋 매핑
		InputComponent->BindKey(EKeys::LeftMouseButton, IE_Pressed, this, &AAZPlayerController::);
		InputComponent->BindAxis("InputAxis", this, &AAZPlayerController::);
		InputComponent->BindAction("InputAction",IE_Pressed, this, &AAZPlayerController::);
		//인헨스드 인풋 매핑
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(UInputAction* , ETriggerEvent::Triggered, this, &Class::Function);
		}
	*/
}

