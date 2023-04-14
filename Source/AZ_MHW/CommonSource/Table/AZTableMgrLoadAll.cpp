#include "AZ_MHW/Manager/AZTableMgr.h"

#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/CommonSource/Table/ConstantData.h"
#include "AZ_MHW/CommonSource/Table/TestData.h"

void UAZTableMgr::LoadAll()
{
	if (!_Load<UConstantData>("ConstantData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UConstantData] Load Fail!")));
	}
	if (!_Load<UTestData>("TestData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UTestData] Load Fail!")));
	}
}
