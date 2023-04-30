// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Util/AZUtility.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"

DEFINE_LOG_CATEGORY(LogShipping);
DEFINE_LOG_CATEGORY(AZMonster);

void UAZUtility::ShippingLog(const FString log_msg)
{
	if (AZGameInstance)
	{
		return;
	}

	UE_LOG(LogShipping, Log, TEXT("%s"), *log_msg);
}

float UAZUtility::MillisecondsToSeconds(const int32 milliseconds)
{
	return milliseconds / 1000.0f;
}

float UAZUtility::PerTenThousandToPerHundred(const int32 per_ten_thousand)
{
	return per_ten_thousand / 100.0f;
}
