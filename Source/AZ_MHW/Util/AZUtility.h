// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum.hpp"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum_switch.hpp"
#include "AZUtility.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogShipping, Log, All);
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZUtility : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "Utility") static void ShippingLog(const FString log_msg);
	template<typename EType>
	static FString EnumToString(EType type)
	{
		auto type_str = magic_enum::enum_name(type);
		FString test(type_str.data());
		return test;
	}
	template<typename EType>
	static EType StringToEnum(const FString& string)
	{
		auto type = magic_enum::enum_cast<EType>(TCHAR_TO_UTF8(*string));
		return type;
	}
	
};
