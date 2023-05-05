// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "type_traits"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum.hpp"
#include "AZ_MHW/CommonSource/MagicEnum/magic_enum_switch.hpp"
#include "AZUtility.generated.h"

#define PRINT_FUNCTION()	UE_LOG(AZMonster, Log, TEXT("Function Called: %s"), __FUNCTIONW__)
DECLARE_LOG_CATEGORY_EXTERN(LogShipping, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(AZMonster, Log, All);

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
		static_assert(std::is_enum_v<EType>, "Non-Enum class is being used");
		auto type_str = magic_enum::enum_name(type);
		FString test(type_str.data());
		return test;
	}
	template<typename EType>
	static EType StringToEnum(const FString& string)
	{
		// Returns the enum value of the string or the first enum element if unavailable
		static_assert(std::is_enum_v<EType>, "Non-Enum class is being used");
		std::string convert_type = TCHAR_TO_ANSI(*string);
		std::optional<EType> type = magic_enum::enum_cast<EType>(convert_type);
		return *type;
	}
	
	template<typename EType>
	static EType StringArrToBitMaskEnum(TArray<FString> enum_string_arr)
	{
		// Converts an array of enum strings into a bit masked enum type
		static_assert(std::is_enum_v<EType>, "Non-Enum class is being used");
		auto enum_value = magic_enum::enum_value<EType>(0);
		
		if (!(enum_string_arr[0] == "NULL" || enum_string_arr[0] == "None"))
		{
			for (const auto enum_str : enum_string_arr)
			{
				enum_value |= UAZUtility::StringToEnum<EType>(enum_str);
			}
		}
		return enum_value;
	}
	
	static float MillisecondsToSeconds(const int32 milliseconds);
	static float PerTenThousandToPerOne(const int32 per_ten_thousand);
};
