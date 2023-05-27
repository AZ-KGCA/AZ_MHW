// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Util/AZUtility.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"

DEFINE_LOG_CATEGORY(AZ_TEST)
DEFINE_LOG_CATEGORY(AZ_MOVE)
DEFINE_LOG_CATEGORY(AZ_ATTACK)
DEFINE_LOG_CATEGORY(AZ_DAMAGE)
DEFINE_LOG_CATEGORY(AZ_PLAYER)
DEFINE_LOG_CATEGORY(LogShipping)
DEFINE_LOG_CATEGORY(AZMonster)

void UAZUtility::ShippingLog(const FString log_msg)
{
	UE_LOG(LogShipping, Log, TEXT("%s"), *log_msg);
}

float UAZUtility::MillisecondsToSeconds(const int32 milliseconds)
{
	return milliseconds / 1000.0f;
}

float UAZUtility::PerTenThousandToPerOne(const int32 per_ten_thousand)
{
	return per_ten_thousand / 10000.0f;
}
