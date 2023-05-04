#include "AZ_MHW/Manager/AZTableMgr.h"

#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/CommonSource/Table/ConstantData.h"
#include "AZ_MHW/CommonSource/Table/DungeonData.h"
#include "AZ_MHW/CommonSource/Table/LoadingData.h"
#include "AZ_MHW/CommonSource/Table/MapData.h"
#include "AZ_MHW/CommonSource/Table/ModelData.h"
#include "AZ_MHW/CommonSource/Table/SpawnData.h"
#include "AZ_MHW/CommonSource/Table/TestData.h"

void UAZTableMgr::LoadAll()
{
	if (!_Load<UConstantData>("ConstantData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UConstantData] Load Fail!")));
	}
	if (!_Load<UDungeonData>("DungeonData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UDungeonData] Load Fail!")));
	}
	if (!_Load<ULoadingData>("LoadingData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[ULoadingData] Load Fail!")));
	}
	if (!_Load<UMapData>("MapData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UMapData] Load Fail!")));
	}
	if (!_Load<UModelData>("ModelData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UModelData] Load Fail!")));
	}
	if (!_Load<USpawnData>("SpawnData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[USpawnData] Load Fail!")));
	}
	if (!_Load<UTestData>("TestData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UTestData] Load Fail!")));
	}
}
