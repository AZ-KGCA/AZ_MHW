#include "AZ_MHW/CommonSource/Table/RangeWeaponData.h"

void URangeWeaponData::Load(TMap<FString, FString>& data)
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
	value_ptr = data.Find("type");
	if (value_ptr != nullptr)
	{
		type = **value_ptr;
	}
	value_ptr = data.Find("damage");
	if (value_ptr != nullptr)
	{
		damage = FCString::Atoi(**value_ptr);
	}
	value_ptr = data.Find("ammo_type");
	if (value_ptr != nullptr)
	{
		TArray<FString> result;
		(*value_ptr).ParseIntoArray(result, TEXT("|"));
		for (auto& str : result)
		{
			ammo_type.Add(str);
		}
	}
	value_ptr = data.Find("critical");
	if (value_ptr != nullptr)
	{
		critical = FCString::Atoi(**value_ptr);
	}
}
