// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Templates/SubclassOf.h"
#include "Engine/StreamableManager.h"
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
	UPROPERTY() class UAZMonsterMgr* monster_mgr_;
	
	UPROPERTY() class UAZResourceMgr* resource_mgr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Classes)
	TSubclassOf<class UAZResourceMgr> resource_mgr_class;

	FStreamableManager streamable_manager;

	
	static UAZGameSingleton* instance();
	void Init();
	void InitResourceMgr();
	class UAZResourceMgr* GetResourceMgr();

public:
	UPROPERTY() float default_scale_factor = -1.0f;
};
#define GetGameSingleton() UAZGameSingleton::instance()