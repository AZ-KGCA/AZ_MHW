// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayerAssetMgr.h"
#include "GameSingleton/AZGameSingleton.h"
#include "Manager/AZTableMgr.h"
#include <AssetRegistry/AssetRegistryModule.h>


UAZPlayerAssetMgr::UAZPlayerAssetMgr()
{
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<
		FAssetRegistryModule>("AssetRegistry");
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
		FString PathPart, ExtensionPart; //안씀
		FPaths::Split(AssetData.GetSoftObjectPath().ToString(), PathPart, AssetName, ExtensionPart);

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

	//Mesh는 ID 고정경로이기 때문에 테이블로 안해도 될 것같다.
	//(물론 회사에서 리소스도 관리를 자주한다면 테이블에서 찾아서 등록하긴 할 듯하다.
	//if(UAZGameSingleton::instance_->table_mgr->_Load<T>("EquipmentData.csv"))
	//{
	//	UAZGameSingleton::instance_->table_mgr->GetData<class Table*>();
	//}
#pragma region BitMask
	//~처리하시지
#pragma endregion
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
	if (const auto& AnimSequence = AnimSequenceMap.Find(Name))
	{
		return *AnimSequence;
	}
	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadSequence(FName Name)
{
	if (AnimSequenceMap.Find(Name))
	{
		AnimSequenceMap.Remove(Name);
		return true;
	}
	return false;
}

UBlendSpace* UAZPlayerAssetMgr::GetBlendSpace(FName Name)
{
	if (const auto& AnimBlendSpace = AnimBlendSpaceMap.Find(Name))
	{
		return *AnimBlendSpace;
	}
	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadBlendSpace(FName Name)
{
	if (AnimBlendSpaceMap.Find(Name))
	{
		AnimBlendSpaceMap.Remove(Name);
		return true;
	}
	return false;
}

UAnimMontage* UAZPlayerAssetMgr::GetMontage(FName Name)
{
	if (const auto& AnimMontage = AnimMontageMap.Find(Name))
	{
		return *AnimMontage;
	}
	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadMontage(FName Name)
{
	if (AnimMontageMap.Find(Name))
	{
		AnimMontageMap.Remove(Name);
		return true;
	}
	return false;
}

USkeletalMesh* UAZPlayerAssetMgr::GetSkeletalMesh(int32 ItemID)
{
	//isn't Mesh ID
	if (ItemID < 1001)
	{
		return nullptr;
	}

	//SKMeshTable
	if (const auto& PlayerCharacterPart = PlayerCharacterPartMap.Find(ItemID))
	{
		return *PlayerCharacterPart;
	}

	
	TMap<FString, FStringFormatArg> PathStringMap;
	PathStringMap.Add("AssetPath",TEXT("/Game/AZ/Character/Mesh"));
	//PathStringMap.Add("GenderType",TEXT("F"));//TEXT("M")
	PathStringMap.Add("AssetType",TEXT("SK"));
	PathStringMap.Add("EquipType",TEXT(""));
	int32 ItemIDPath;
	
	if (ItemID < 10000)
	{
		if (ItemID < 1501)//대검
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP00");
			ItemIDPath = ItemID - 1000;
		}
		else if (ItemID < 2001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP01");
			ItemIDPath = ItemID - 1500;
		}
		else if (ItemID < 2501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP02");
			ItemIDPath = ItemID - 2000;
		}
		else if (ItemID < 3001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP03");
			ItemIDPath = ItemID - 2500;
		}
		else if (ItemID < 3501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP04");
			ItemIDPath = ItemID - 3000;
		}
		else if (ItemID < 4001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP05");
			ItemIDPath = ItemID - 3500;
		}
		else if (ItemID < 4501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP06");
			ItemIDPath = ItemID - 4000;
		}
		else if (ItemID < 5001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP07");
			ItemIDPath = ItemID - 4500;
		}
		else if (ItemID < 5501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP08");
			ItemIDPath = ItemID - 5000;
		}
		else if (ItemID < 6001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP09");
			ItemIDPath = ItemID - 5500;
		}
		else if (ItemID < 6501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP10");
			ItemIDPath = ItemID - 6000;
		}
		else if (ItemID < 7001)//활
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP11");
			ItemIDPath = ItemID - 6500;
		}
		else if (ItemID < 7501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP12");
			ItemIDPath = ItemID - 7000;
		}
		else if (ItemID < 8001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("WP13");
			ItemIDPath = ItemID - 7500;
		}
	}
	else if (ItemID < 12600)
	{
		if (ItemID < 10501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("FBody");
			ItemIDPath = ItemID - 10000;
		}
		else if (ItemID < 11001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("FLeg");
			ItemIDPath = ItemID - 10500;
		}
		else if (ItemID < 11501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("FArm");
			ItemIDPath = ItemID - 11000;
		}
		else if (ItemID < 12001)
		{
			PathStringMap["EquipType"] = FStringFormatArg("FWaist");
			ItemIDPath = ItemID - 11500;
		}
		else if (ItemID < 12501)
		{
			PathStringMap["EquipType"] = FStringFormatArg("FHead");
			ItemIDPath = ItemID - 12000;
		}
		else if (ItemID < 12601)
		{
			PathStringMap["EquipType"] = FStringFormatArg("FHair");
			ItemIDPath = ItemID - 12400;
		}
		else
		{
			return nullptr;
		}
	}
	
	FString LoadPath = FString::Format(TEXT("{AssetPath}/{EquipType}/{AssetType}_{EquipType}_"), PathStringMap);
	LoadPath += FString::Printf(TEXT("%03d"), ItemIDPath);
	USkeletalMesh* LoadResult = LoadObject<USkeletalMesh>(nullptr, *LoadPath, nullptr, LOAD_None, nullptr);
	if (LoadResult)
	{
		PlayerCharacterPartMap.Add(ItemID, LoadResult);
		return LoadResult;
	}

	return nullptr;
}
bool UAZPlayerAssetMgr::UnloadSkeletalMesh(int32 ItemID)
{
	if (PlayerCharacterPartMap.Find(ItemID))
	{
		//PlayerCharacterPartMap[ID] = nullptr;
		PlayerCharacterPartMap.Remove(ItemID);
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
	if (const auto& BitMask = CommandBitMaskMap.Find(Name))
	{
		return *BitMask;
	}
	return 0;
}
