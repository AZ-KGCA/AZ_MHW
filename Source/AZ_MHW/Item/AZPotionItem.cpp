// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPotionItem.h"
#include "AZItemData.h"

void UAZPotionItem::InitItem(FPotionInfo& new_info)
{
	info = &new_info;
}

void UAZPotionItem::SetCount(int32 count)
{
	info->item_count = count;
}

void UAZPotionItem::IncreaseCount(int32 count)
{
	info->item_count = info->item_count + count;
}

void UAZPotionItem::DecreaseCount(int32 count)
{
	info->item_count = info->item_count - count;
	if(info->item_count <= 0)
	{
		info->item_count = 0;
	}
}

FPotionInfo* UAZPotionItem::GetItemInfo() const
{
	return info;
}

int32 UAZPotionItem::GetItemCount()
{
	return info->item_count;
}

int32 UAZPotionItem::GetItemKey()
{
	return info->item_key;
}

EStorageType UAZPotionItem::GetItemStorageType()
{
	return info->storage_type;
}
