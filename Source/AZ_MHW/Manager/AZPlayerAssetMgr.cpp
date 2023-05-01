// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayerAssetMgr.h"
#include <AssetRegistry/AssetRegistryModule.h>

UAZPlayerAssetMgr::UAZPlayerAssetMgr()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
#pragma region AnimSequence
//씨퀀스용 애니메이션(강제)
//제스쳐용 애니메이션
	
#pragma endregion
#pragma region AnimBlendSpace
//정지상태에서 회전할 경우
//기본포즈용 시퀀스를 자동할당(블렌드스페이스에서 가져오기)
#pragma endregion
#pragma region AnimMontage
//일반적인 모든 액션상태
//기본포즈용 시퀀스를 자동할당
	const FString InputActionDirectory = TEXT("/Game/AZ/Character/Animation/");
	// 에셋 레지스트리 모듈을 얻음
	
	
	// 디렉토리 내의 모든 에셋을 가져옴
	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByPath(*InputActionDirectory, AssetDataList, false);

	// 에셋을 로드함
	for (const FAssetData& AssetData : AssetDataList)
	{
		// 에셋 경로로부터 에셋 이름을 추출함
		FString AssetName;
		FString PathPart, ExtensionPart;//안씀
		FPaths::Split(AssetData.GetSoftObjectPath().ToString(), PathPart, AssetName,ExtensionPart);
		
		// 에셋을 로드함
		//if (UInputAction* InputAction = Cast<UInputAction>(AssetData.GetAsset()))
		//{
		// InputActionMap에 추가함
		/*
		 *에셋이름 제거(IA_)//상수로 3입력
		const FString AssetsPrefix = "IA_";
		int32 PrefixSize =AssetsPrefix.Len();
		InputActionMap.Add(*AssetName.Mid(PrefixSize), InputAction);
		*/
		//InputActionMap.Add(*AssetName.Mid(3), InputAction);
		//}
	}
#pragma endregion
	FModuleManager::Get().UnloadModule("AssetRegistry");
}

void UAZPlayerAssetMgr::BeginDestroy()
{
	UObject::BeginDestroy();
	AnimBlendSpaceMap.Empty();
	AnimMontageMap.Empty();
	AnimSequenceMap.Empty();
	PlayerCharacterPartMap.Empty();
	CommandBitMaskMap.Empty();
}

UAnimSequence* UAZPlayerAssetMgr::GetSequence(FName Name)
{
	if(const auto& AnimSequence = AnimSequenceMap.Find(Name))
	{
		return *AnimSequence;
	}
	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadSequence(FName Name)
{
	if(AnimSequenceMap.Find(Name))
	{
		AnimSequenceMap.Remove(Name);
		return true;
	}
	return false;
}
UBlendSpace* UAZPlayerAssetMgr::GetBlendSpace(FName Name)
{
	if(const auto& AnimBlendSpace = AnimBlendSpaceMap.Find(Name))
	{
		return *AnimBlendSpace;
	}
	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadBlendSpace(FName Name)
{
	if(AnimBlendSpaceMap.Find(Name))
	{
		AnimBlendSpaceMap.Remove(Name);
		return true;
	}
	return false;
}
UAnimMontage* UAZPlayerAssetMgr::GetMontage(FName Name)
{
	if(const auto& AnimMontage = AnimMontageMap.Find(Name))
	{
		return *AnimMontage;
	}
	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadMontage(FName Name)
{
	if(AnimMontageMap.Find(Name))
	{
		AnimMontageMap.Remove(Name);
		return true;
	}
	return false;
}
USkeletalMesh* UAZPlayerAssetMgr::GetSkeletalMesh(FName Name)
{
	if(const auto& PlayerCharacterPart = PlayerCharacterPartMap.Find(Name))
	{
		return *PlayerCharacterPart;
	}
	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadSkeletalMesh(FName Name)
{
	if(PlayerCharacterPartMap.Find(Name))
	{
		PlayerCharacterPartMap.Remove(Name);
		return true;
	}
	return false;
}
void UAZPlayerAssetMgr::SetCommandBitMaskMap(FName WPName)
{
	CommandBitMaskMap.Empty();
	//Load
	//for(LoadCount)
	{
		//CommandBitMaskMap.Add(,);
	}
}

uint32 UAZPlayerAssetMgr::GetCommandBitMask(FName Name)
{
	if(const auto& BitMask =CommandBitMaskMap.Find(Name))
	{
		return *BitMask;
	}
	return 0;
}