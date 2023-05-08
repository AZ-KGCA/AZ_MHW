// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
<<<<<<< HEAD
#include "AZ_MHW/Manager/AZResourceMgr.h"
=======
#include "AZ_MHW/Manager/AZPlayerAssetMgr.h"
>>>>>>> origin/feature/Character

UAZGameSingleton* UAZGameSingleton::instance_ = nullptr;

UAZGameSingleton::UAZGameSingleton()
{
	resource_mgr = nullptr;
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
	InitResourceMgr();

	table_mgr = NewObject<UAZTableMgr>();
	table_mgr->LoadAll();
	table_mgr->LoadComplete();

<<<<<<< HEAD
	monster_mgr = NewObject<UAZMonsterMgr>();
	monster_mgr->Init();
}

void UAZGameSingleton::InitResourceMgr()
{
	if (resource_mgr == nullptr)
	{
		UClass* object_class = FSoftClassPath(TEXT("/Game/Blueprint/Resource/BP_ResourceMgr.BP_ResourceMgr_C")).TryLoadClass<UAZResourceMgr>();

		if (object_class)
		{
			resource_mgr = NewObject<UAZResourceMgr>(this, object_class);
		}
		else
		{
			resource_mgr = NewObject<UAZResourceMgr>();
		}

		if (resource_mgr != nullptr)
		{
			resource_mgr->Init();
		}
	}
}

UAZResourceMgr* UAZGameSingleton::GetResourceMgr()
{
	return resource_mgr;
=======
	player_asset_mgr_ = NewObject<UAZPlayerAssetMgr>();
	//player_asset_mgr
>>>>>>> origin/feature/Character
}