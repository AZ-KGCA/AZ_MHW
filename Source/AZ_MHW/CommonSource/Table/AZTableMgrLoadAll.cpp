#include "AZ_MHW/Manager/AZTableMgr.h"

#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/CommonSource/Table/ConstantData.h"
#include "AZ_MHW/CommonSource/Table/TestData.h"
#include "AZ_MHW/CommonSource/Table/MonsterCombatActionData.h"
#include "AZ_MHW/CommonSource/Table/MonsterNonCombatActionData.h"
#include "AZ_MHW/CommonSource/Table/BossData.h"
#include "AZ_MHW/CommonSource/Table/MonsterData.h"

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
	if (!_Load<UMonsterCombatActionData>("MonsterCombatActionData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UMonsterCombatActionData] Load Fail!")));
	}
	if (!_Load<UMonsterNonCombatActionData>("MonsterNonCombatActionData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UMonsterNonCombatActionData] Load Fail!")));
	}
	if (!_Load<UBossData>("BossData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UBossData] Load Fail!")));
	}
	if (!_Load<UMonsterData>("MonsterData.csv"))
	{
		UAZUtility::ShippingLog(FString::Printf(TEXT("[UMonsterData] Load Fail!")));
	}
}
