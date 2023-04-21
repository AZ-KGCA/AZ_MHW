// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayerController.h"
#include "GameInstance/AZGameInstance.h"
#include "Manager/AZInputMgr.h"//AZGameInstance.h
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>


AAZPlayerController::AAZPlayerController()
{
	
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

void AAZPlayerController::AddInputMappingContext(FName IMCName) const
{
	UAZInputMgr* InputMgr = AZGameInstance->input_mgr;//GetGameInstance<UAZGameInstance>()->input_mgr;
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(const auto& InputMappingContext = InputMgr->InputMappingContextMap.Find(IMCName))
		{
			if(const auto& ContextPriority = InputMgr->InputMappingContextPriorityMap.Find(IMCName))
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(*InputMappingContext, *ContextPriority);
			}
		}
		else
		{
			PRINT_LOG("Don't Add InputMappingContext");
			//인풋매핑컨텍스트 없음
		}
	}
}

void AAZPlayerController::RemoveInputMappingContext(FName IMCName) const
{
	UAZInputMgr* InputMgr = AZGameInstance->input_mgr;
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if(const auto& InputMappingContext = InputMgr->InputMappingContextMap.Find(IMCName))
		{
			EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(*InputMappingContext);
		}
		else
		{
			PRINT_LOG("Don't Find InputMappingContext");
			//인풋매핑컨텍스트 없음
		}
	}
}

void AAZPlayerController::ClearInputMappingContext() const
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		EnhancedInputLocalPlayerSubsystem ->ClearAllMappings();
	}
	else
	{
		PRINT_LOG("Don't Find EnhancedInputLocalPlayerSubsystem");
		//인풋매핑컨텍스트 없음
	}
}
