// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Util/AZUtility.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"

DEFINE_LOG_CATEGORY(LogShipping);

void UAZUtility::ShippingLog(const FString log_msg)
{
	if (AZGameInstance)
	{
		return;
	}

	UE_LOG(LogShipping, Log, TEXT("%s"), *log_msg);
}