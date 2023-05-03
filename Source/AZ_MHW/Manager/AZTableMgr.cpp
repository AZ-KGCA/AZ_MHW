// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZTableMgr.h"

#include "AZ_MHW/CommonSource/Table/ConstantData.h"
#include "AZ_MHW/CommonSource/Table/TestData.h"

void UAZTableMgr::LoadComplete()
{
	const UTestData* test1 = GetData<UTestData>(1);
	TArray<const UTestData*> test_results;
	test_results = GetData<UTestData>([](const UTestData* check) {
		if (check->test1 == true)
		{
			return true;
		}
		return false;
		});
}