#include "AZ_MHW/CommonSource/Table/TotalItemData.h"

void UTotalItemData::Load(TMap<FString, FString>& data)
{
	FString* value_ptr = nullptr;
	value_ptr = data.Find("id");
	if (value_ptr != nullptr)
	{
		id = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("name");
	if (value_ptr != nullptr)
	{
		name = **value_ptr;
	}
	value_ptr = data.Find("value");
	if (value_ptr != nullptr)
	{
		value = **value_ptr;
	}
	value_ptr = data.Find("warehouse_max");
	if (value_ptr != nullptr)
	{
		warehouse_max = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("pocket_max");
	if (value_ptr != nullptr)
	{
		pocket_max = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("type");
	if (value_ptr != nullptr)
	{
		type = **value_ptr;
	}
}
