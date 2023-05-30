// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZGameCacheInfo.h"

#include "Internationalization/TextKey.h"

int32 UAZGameCacheInfo::alloc_character_index_ = 0;

UAZGameCacheInfo::UAZGameCacheInfo()
{
}

void UAZGameCacheInfo::Init()
{
}

uint32 UAZGameCacheInfo::GetIDHashCode(FString& id)
{
	auto hash_code = TextKeyUtil::HashString(*id);
	return hash_code;
}

bool UAZGameCacheInfo::GetID(int32 id_hash, FString& out_id)
{
	auto find_id = id_check_.Find(id_hash);
	if (find_id == nullptr)
	{
		return false;
	}

	out_id = *find_id;
	return true;
}

TArray<FCharacterSimpleInfo> UAZGameCacheInfo::GetCharacterSimpleInfo(uint32 id)
{
	auto character_infos = character_info_.Find(id);
	if (character_infos == nullptr)
	{
		return TArray<FCharacterSimpleInfo>();
	}
	return *character_infos;
}

UAZInventoryManager* UAZGameCacheInfo::GetInventoryManager(int32 character_index)
{
	auto character_info = character_inventory_.Find(character_index);
	if (character_info == nullptr)
	{
		return nullptr;
	}
	return *character_info;
}

bool UAZGameCacheInfo::SignupRequest(FString& id)
{
	auto hash_code = GetIDHashCode(id);
	auto temp_string = id_check_.Find(hash_code);
	// 1. 이미 아이디가 존재
	if (temp_string != nullptr)
	{
		return false;
	}

	id_check_.Add(hash_code, id);
	return true;
}
