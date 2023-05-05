// Fill out your copyright notice in the Description page of Project Settings.


#include "AZAmmoItem.h"
#include "AZItemData.h"

void UAZAmmoItem::InitItem(FAmmoInfo& item_info)
{
	info = &item_info;
}

void UAZAmmoItem::IncreaseCount(int32 count)
{
	info->item_count = info->item_count + count;
}

void UAZAmmoItem::DecreaseCount(int32 count)
{
	info->item_count = info->item_count - count;
}

int32 UAZAmmoItem::GetItemCount()
{
	return info->item_count;
}

int32 UAZAmmoItem::GetItemKey()
{
	return info->item_key;
}

FAmmoInfo* UAZAmmoItem::GetItemInfo()
{
	return info;
}

EStorageType UAZAmmoItem::GetStorageType()
{
	return info->storage_type;
}
