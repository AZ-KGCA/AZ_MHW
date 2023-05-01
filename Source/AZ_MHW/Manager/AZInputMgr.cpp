// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/AZInputMgr.h"
#include <InputMappingContext.h>
#include <AssetRegistry/AssetRegistryModule.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

UAZInputMgr::UAZInputMgr()
{
	// 에셋 레지스트리 모듈을 얻음
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
#pragma region InputMappingContext_로드
	{
		const FString InputMappingContextDirectory = TEXT("/Game/AZ/DataAsset/Input/InputMappingContext");
		
		// 디렉토리 내의 모든 에셋을 가져옴
		TArray<FAssetData> AssetDataList;
		AssetRegistryModule.Get().GetAssetsByPath(*InputMappingContextDirectory, AssetDataList, true);

		// 에셋을 로드함
		for (const FAssetData& AssetData : AssetDataList)
		{
			// 에셋 경로로부터 에셋 이름을 추출함
			FString AssetName;
			FString PathPart, ExtensionPart;
			FPaths::Split(AssetData.GetSoftObjectPath().ToString(), PathPart, AssetName,ExtensionPart);
			
			// 에셋을 로드함
			if (UInputMappingContext* InputMappingContext = Cast<UInputMappingContext>(AssetData.GetAsset()))
			{
				// InputMappingContextMap에 추가함
				/** 에셋이름 제거(IA_)//상수로 4입력
				 * const FString AssetsPrefix = "IMC_";
				 * int32 PrefixSize = AssetsPrefix.Len();
				 * InputActionMap.Add(*AssetName.Mid(PrefixSize), InputAction);
				*/
				InputMappingContextMap.Add(*AssetName.Mid(4), InputMappingContext);
				// InputMappingContextPriorityMap에 추가함
				/** 경로상의 상위 폴더를 숫자폴더로 만들어서 저장
				 * 경로상 숫자가 아니라면 자동 0으로 로드됨.
				 * InputMappingContext/InGame/0/IMC_XXXXXX;
				 * InputMappingContext/Lobby/0/IMC_XXXXXX;
				 * InputMappingContext/InGame/1/IMC_XXXXXX;
				 * 파일이름에 넣어도 사실 상관없다고는 생각하긴하는데..
				 */
				FString PriorityPart = PathPart.Right(1);
				int32 Priority = FCString::Atoi(*PriorityPart);
				InputMappingContextPriorityMap.Add(*AssetName.Mid(4), Priority);
			}
		}
	}
#pragma endregion
#pragma region InputAction_로드
	{
		const FString InputActionDirectory = TEXT("/Game/AZ/DataAsset/Input/InputAction");
		// 에셋 레지스트리 모듈을 얻음
		
	
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
		//EnhancedInputComponent->BindAction(GetInputAction("InputActionName"), ETriggerEvent::Triggered,this, &UAZInputMgr::ActionTest);
	}
}

UInputMappingContext* UAZInputMgr::GetInputMappingContext(const FName& Name)
{
	if(auto InputMappingContext = InputMappingContextMap.Find(Name))
	{
		return *InputMappingContext;
	}
	return nullptr;
}
int32* UAZInputMgr::GetInputMappingContextPriority(const FName& Name)
{
	if(auto InputMappingContextPriority = InputMappingContextPriorityMap.Find(Name))
	{
		return InputMappingContextPriority;
	}
	return nullptr;
}
UInputAction* UAZInputMgr::GetInputAction(const FName& Name)
{
	if(auto InputAction = InputActionMap.Find(Name))
	{
		return *InputAction;
	}
	return nullptr;
}

void UAZInputMgr::AddInputMappingContext(const FName& IMCName, const ULocalPlayer* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		if(const auto InputMappingContext = GetInputMappingContext(IMCName))
		{
			if(const auto ContextPriority = GetInputMappingContextPriority(IMCName))
			{
				EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, *ContextPriority);
			}
		}
		else
		{
			PRINT_LOG("Don't Add InputMappingContext");
			//인풋매핑컨텍스트 없음
		}
	}
	else
	{
		PRINT_LOG("Don't Find EnhancedInputLocalPlayerSubsystem");
	}
}
void UAZInputMgr::RemoveInputMappingContext(const FName& IMCName, const ULocalPlayer* LocalPlayer)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		if(const auto InputMappingContext = GetInputMappingContext(IMCName))
		{
			EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		}
		else
		{
			PRINT_LOG("Don't Find InputMappingContext");
			//인풋매핑컨텍스트 없음
		}
	}
	else
	{
		PRINT_LOG("Don't Find EnhancedInputLocalPlayerSubsystem");
	}
}
void UAZInputMgr::ClearInputMappingContext(const ULocalPlayer* LocalPlayer) const
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		EnhancedInputLocalPlayerSubsystem ->ClearAllMappings();
	}
	else
	{
		PRINT_LOG("Don't Find EnhancedInputLocalPlayerSubsystem");
	}
}

void UAZInputMgr::ActionTest()
{
	PRINT_LOG("InputBinding Test");
}