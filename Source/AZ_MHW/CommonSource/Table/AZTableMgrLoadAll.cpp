#include "AZ_MHW/Manager/AZTableMgr.h"

#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/CommonSource/Table/BottleData.h"
#include "AZ_MHW/CommonSource/Table/ConstantData.h"
#include "AZ_MHW/CommonSource/Table/ItemBuffData.h"
#include "AZ_MHW/CommonSource/Table/PotionData.h"
#include "AZ_MHW/CommonSource/Table/TestData.h"
#include "AZ_MHW/CommonSource/Table/TotalItemData.h"

void UAZTableMgr::LoadAll()
{
	if (!_Load<UBottleData>("BottleData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UBottleData] Load Fail!")));
	}
	if (!_Load<UConstantData>("ConstantData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UConstantData] Load Fail!")));
	}
	if (!_Load<UItemBuffData>("ItemBuffData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UItemBuffData] Load Fail!")));
	}
	if (!_Load<UPotionData>("PotionData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UPotionData] Load Fail!")));
	}
	if (!_Load<UTestData>("TestData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UTestData] Load Fail!")));
	}
	if (!_Load<UTotalItemData>("TotalItemData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UTotalItemData] Load Fail!")));
	}
}
