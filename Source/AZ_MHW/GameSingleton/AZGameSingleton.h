// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameSingleton : public UObject
{
	GENERATED_BODY()
public:
	UAZGameSingleton();
	
private:
	static UAZGameSingleton* instance_;

public:
	UPROPERTY() class UAZTableMgr* table_mgr;
	UPROPERTY() class UAZPlayerAssetMgr* player_asset_mgr_;
	static UAZGameSingleton* instance();
	void Init();
};
