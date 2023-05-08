// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Map/AZMapStatics.h"

#include "AZ_MHW/Manager/AZMapMgr.h"
#include "AZ_MHW/CommonSource/Table/SpawnData.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"

TArray<FSpawnObjectInfo> UAZMapStatics::GetCurMapSpawnObjectInfo()
{
	return GetMapSpawnObjectInfo(AZGameInstance->map_mgr->GetMapIndex());
}

TArray<FSpawnObjectInfo> UAZMapStatics::GetMapSpawnObjectInfo(int32 map_index)
{
	// �⺻ ���� ������Ʈ ����
	auto spawn_info_array = GetTable(USpawnData);
	auto area_object_list = spawn_info_array.FilterByPredicate([map_index](const USpawnData* data)
		{
			return data->map_index[0] == map_index && data->is_active;
		});

	TArray<FSpawnObjectInfo> spawn_object_list;
	for (const USpawnData* spawn_data : area_object_list)
	{
		int32 max_count = spawn_data->obj_index.Num();
		for (int32 i = 0; i < max_count; ++i)
		{
			int32 object_index = spawn_data->obj_index[i];
			//FIXME ���� �۾��� �߰��ϱ�
		}
	}

	return TArray<FSpawnObjectInfo>();
}
