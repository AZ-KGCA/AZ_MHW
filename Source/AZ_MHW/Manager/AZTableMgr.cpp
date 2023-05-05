// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Manager/AZTableMgr.h"

#include "AZ_MHW/CommonSource/Table/ConstantData.h"
#include "AZ_MHW/CommonSource/Table/TestData.h"

void UAZTableMgr::LoadComplete()
{
	/*const UTestData* test1 = GetData<UTestData>(1);
	TArray<const UTestData*> test_results;*/
	/*total_item_array = GetData<UTOTAL_ITEM_TABLE>([](const UTOTAL_ITEM_TABLE* check) {
		if (check->count != 0)
		{
			return true;
		}
		return false;
		});*/
	LoadTotalItemTable();
	LoadConsumeTable();
	LoadBuffTable();
}

void UAZTableMgr::LoadTotalItemTable()
{
	total_item_array = GetData<UTotalItemData>([](const UTotalItemData* total_item) {
	if (total_item->type != "hair")
	{
		return true;
	}
	return false;
	});
}

void UAZTableMgr::LoadConsumeTable()
{
	
	consume_item_array = GetData<UPotionData>([](const UPotionData* consume_item)
	{
		return true;
	});

}

void UAZTableMgr::LoadBuffTable()
{
	buff_array =GetData<UItemBuffData>([](const UItemBuffData* buff)
	{
		return true;
	});
}
