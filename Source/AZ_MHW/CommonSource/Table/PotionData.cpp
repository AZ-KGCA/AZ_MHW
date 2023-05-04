#include "AZ_MHW/CommonSource/Table/PotionData.h"

void UPotionData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("buff_type");
	if (value_ptr != nullptr)
	{
		buff_type = **value_ptr;
	}
	value_ptr = data.Find("usable");
	if (value_ptr != nullptr)
	{
		usable = (*value_ptr).ToBool();
	}
}
