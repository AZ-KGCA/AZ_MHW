// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/Manager/SaveData/GameOptionSaveData.h"
#include "AZGameOption.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameOption : public UObject
{
	GENERATED_BODY()
public:
	static FGameOptionSaveData* option_save_data;

public:
	void Init();

	// AZHUD_Lancher에서 호출
	void InitGameOption();
};
