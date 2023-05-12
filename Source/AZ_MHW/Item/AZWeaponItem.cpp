// Fill out your copyright notice in the Description page of Project Settings.


#include "AZWeaponItem.h"
#include "AZItemData.h"
void UAZWeaponItem::InitItem(FWeaponInfo& info)
{
	info_ = info;
}

EWeaponType UAZWeaponItem::GetWeaponType()
{
	return info_.weapon_type;
}

FWeaponInfo UAZWeaponItem::GetWeaponInfo()
{
	return info_;
}

bool UAZWeaponItem::GetEquipState()
{
	return info_.is_equip;
}