// Fill out your copyright notice in the Description page of Project Settings.


#include "AZArmorItem.h"
#include "AZItemData.h"

void UAZArmorItem::InitItem(FArmorInfo& info)
{
	info_ = &info;
}

FArmorInfo* UAZArmorItem::GetItemInfo()
{
	return info_;
}
