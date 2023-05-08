#include "AZ_MHW/CommonSource/Table/BottleData.h"

void UBottleData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
	}
	value_ptr = data.Find("buff_type");
	if (value_ptr != nullptr)
	{
	}
}
