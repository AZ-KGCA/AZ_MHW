// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Manager/AZGameOption.h"

#include "AZ_MHW/Manager/SaveData/GameOptionSaveData.h"

FGameOptionSaveData* UAZGameOption::option_save_data = nullptr;

void UAZGameOption::Init()
{
}

void UAZGameOption::InitGameOption()
{
	// FIXME (추후 옵션 로드 작업 하기)
}
