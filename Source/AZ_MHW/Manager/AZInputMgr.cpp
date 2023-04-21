// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/AZInputMgr.h"
#include <InputMappingContext.h>
#include <AssetRegistry/AssetRegistryModule.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

UAZInputMgr::UAZInputMgr()
{
	//TODO: IMC 로드부 임시구현
	//InputMappingContext CSV 로 이름과 에셋경로로 맵핑컨텍스트들을 저장한다.
	//while(eof)
	{
		//발도상태
		FString CSVName = "Melee";
		FString CSVPriority = "3";
		FString CSVPath = "/Game/AZ/DataAsset/Input/InputMappingContext/InGame/IMC_MeleeWeapons";
		
		UInputMappingContext* Imc = LoadObject<UInputMappingContext>(nullptr, *CSVPath);
		if(Imc) {
			const FName ContextName = FName(*CSVName);
			InputMappingContextPriorityMap.Add(ContextName, FCString::Atoi(*CSVPriority));
			InputMappingContextMap.Add(ContextName, Imc); 
		}
		//임시 반복(카메라 회전, 이동 등)
				CSVName = "InGame";
				CSVPriority = "1";
				CSVPath = "/Game/AZ/DataAsset/Input/InputMappingContext/InGame/IMC_InGame";
		
				Imc = LoadObject<UInputMappingContext>(nullptr, *CSVPath);

				if (Imc) {
					const FName ContextName = FName(*CSVName);
					InputMappingContextPriorityMap.Add(ContextName, FCString::Atoi(*CSVPriority));
					InputMappingContextMap.Add(ContextName, Imc);
				}
		//
		//임시 반복(납도 상태)
				CSVName = "Sheathe";
				CSVPriority = "2";
				CSVPath = "/Game/AZ/DataAsset/Input/InputMappingContext/InGame/IMC_SheatheWeapons";
				
				Imc = LoadObject<UInputMappingContext>(nullptr, *CSVPath);

				if (Imc) {
					const FName ContextName = FName(*CSVName);
					InputMappingContextPriorityMap.Add(ContextName, FCString::Atoi(*CSVPriority));
					InputMappingContextMap.Add(ContextName, Imc);
				}
		//
	}

	//InputAction CSV 로 이름과 에셋경로로 맵핑컨텍스트들을 저장한다.
	//TODO: IA 로드부 임시구현
#pragma region InputAction_로드
	const FString InputActionDirectory = TEXT("/Game/AZ/DataAsset/Input/InputAction");
	// 에셋 레지스트리 모듈을 얻음
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	
	// 디렉토리 내의 모든 에셋을 가져옴
	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByPath(*InputActionDirectory, AssetDataList, true);

	// 에셋을 로드함
	for (const FAssetData& AssetData : AssetDataList)
	{
		// 에셋 경로로부터 에셋 이름을 추출함
		FString AssetName;
		FString PathPart, ExtensionPart;//안씀
		FPaths::Split(AssetData.GetSoftObjectPath().ToString(), PathPart, AssetName,ExtensionPart);
		
		// 에셋을 로드함
		if (UInputAction* InputAction = Cast<UInputAction>(AssetData.GetAsset()))
		{
			// InputActionMap에 추가함
			/*
			 *에셋이름 제거(IA_)//상수로 3입력
			const FString AssetsPrefix = "IA_";
			int32 PrefixSize =AssetsPrefix.Len();
			InputActionMap.Add(*AssetName.Mid(PrefixSize), InputAction);
			*/
			InputActionMap.Add(*AssetName.Mid(3), InputAction);
		}
	}
	//사용한 에셋 레지스트리 모듈 해제
	//(다른곳에서도 이것을 사용하고 있는데 해제한다면? 애초에 계속 로드해놓을 필요가 없는 모듈.)
	FModuleManager::Get().UnloadModule("AssetRegistry");
#pragma endregion
	
}

void UAZInputMgr::BeginDestroy()
{
	Super::BeginDestroy();
	InputMappingContextPriorityMap.Empty();
	InputMappingContextMap.Empty();
	InputActionMap.Empty();
}

//PlayerController가 UInputComponent 소유하고 있다. PlayerController에서 아래 컨텍스트를 호출
//Controller->ClearInputMappingContext(); //초기화
//Controller->AddInputMappingContext("ContextName"); //등록
//"InputMgrInstance"->SetupDefaultInput(InputComponent); //호출
//Controller->BindAction
void UAZInputMgr::SetupDefaultBindAction(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->ClearActionBindings();
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
		//EnhancedInputComponent->BindAction(DEREF_IAMAP("InputActionName"),ETriggerEvent::Triggered,this,&UAZInputMgr::ActionTest);
	}
}

void UAZInputMgr::ActionTest()
{
	PRINT_LOG("InputBinding Test");
}