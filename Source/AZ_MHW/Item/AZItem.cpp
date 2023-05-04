/*
// Fill out your copyright notice in the Description page of Project Settings.


#include "AZItem.h"

void UAZItem::InitItem(const FItemInfo &new_info)
{
	item_info = new_info;
}

void UAZItem::SetItemCount(int32 count)
{
	item_info.item_count = count;
}

FItemInfo* UAZItem::GetItemInfo()const
{
	return const_cast<FItemInfo*>(&item_info);
}

int UAZItem::GetItemCount()
{
	return item_info.item_count;
}

int32 UAZItem::GetItemIndex()
{
	return item_info.item_index;
}

EStorageType UAZItem::GetItemStorageType()
{
	return item_info.storage_type;
}

void UAZItem::IncreaseCount(int32 count)
{
	item_info.item_count += count;
}

void UAZItem::DecreaseCount(int32 count)
{
	item_info.item_count -= count;
	if(item_info.item_count <=0)
	{
		item_info.item_count = 0;
	}
}
*/
