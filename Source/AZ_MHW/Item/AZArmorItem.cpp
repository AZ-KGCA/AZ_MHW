// Fill out your copyright notice in the Description page of Project Settings.


#include "AZArmorItem.h"
#include "AZItemData.h"
#include "IWebBrowserWindow.h"

void UAZArmorItem::InitItem(FArmorInfo& info)
{
	info_ = info;
}

FArmorInfo UAZArmorItem::GetItemInfo()
{
	return info_;
}

EArmorType UAZArmorItem::GetArmorType()
{
	return info_.armor_type;
}

bool UAZArmorItem::GetEquipState()
{
	return info_.is_equip;
}
