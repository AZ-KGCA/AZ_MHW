// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZTableMgr.h"

UAZGameSingleton* UAZGameSingleton::instance_ = nullptr;

UAZGameSingleton::UAZGameSingleton()
{

}

UAZGameSingleton* UAZGameSingleton::instance()
{
	if (instance_ == nullptr)
	{
		instance_ = Cast<UAZGameSingleton>(GEngine->GameSingleton);
		if (instance_ && instance_->IsValidLowLevel())
		{
			instance_->Init();
		}
	}
	return instance_;
}

void UAZGameSingleton::Init()
{
	table_mgr = NewObject<UAZTableMgr>();
	table_mgr->LoadAll();
	table_mgr->LoadComplete();
}