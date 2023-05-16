// Fill out your copyright notice in the Description page of Project Settings.


#include "AZInventoryManager.h"
#include "AZTableMgr.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "Util/AZUtility.h"

void UAZInventoryManager::Init()
{
	instance_ = UAZGameSingleton::instance();
	
	ResetMgr();
	SetMaxCount();
	GetTableData();
	CreateStartItem();
	SetPotionSlot();
	SetBottleSlot();
}

void UAZInventoryManager::ResetMgr()
{
	potion_pocket_.Empty();
	potion_warehouse_.Empty();
}

void UAZInventoryManager::SetMaxCount()
{
	potion_warehouse_max_count = 50;
	potion_pocket_max_count = 15;
	bottle_warehouse_max_count = 20;
	bottle_pocket_max_count = 10;
}

void UAZInventoryManager::GetTableData()
{
	for(auto total_item : instance_->table_mgr->total_item_array_)
	{
		FTotalItemDataStruct total_data;
		total_data.id = total_item->id;
		total_data.name = total_item->name;
		total_data.warehouse_max = total_item->warehouse_max;
		total_data.pocket_max = total_item->pocket_max;
		FString type = total_item->type;
		total_data.type = UAZUtility::StringToEnum<EItemType>(type);
		total_data_map_.Emplace(total_data.id, total_data);
		UE_LOG(LogTemp,Warning,TEXT("%s"),*total_data.name);
	}
	
	for(auto potion_data : instance_->table_mgr->potion_item_array_)
	{
		FPotionDataStruct potion_struct;
		potion_struct.id = potion_data->id;
		potion_struct.is_buff = potion_data->is_buff;
		potion_struct.is_usable = potion_data->usable;
		potion_data_map_.Emplace(potion_struct.id, potion_struct);
		
	}
	for(auto bottle_data : instance_->table_mgr->bottle_array_)
	{
		FBottleDataStruct bottle_struct;
		bottle_struct.id = bottle_data->id;
		bottle_struct.is_buff = bottle_data->is_buff;
		bottle_data_map_.Emplace(bottle_struct.id, bottle_struct);
	}
	for(auto buff_data : instance_->table_mgr->buff_array_)
	{
		FBuffDataStruct buff_struct;
		buff_struct.id = buff_data->id;
		buff_struct.effect = buff_data->effect;
		buff_struct.target = UAZUtility::StringToEnum<EItemTarget>(buff_data->target);
		buff_struct.calc = UAZUtility::StringToEnum<ECalculation>(buff_data->calculation);
		buff_data_map_.Emplace(buff_struct.id,buff_struct);
	}
	for(auto melee_weapon : instance_->table_mgr->melee_weapon_array_)
	{
		FMeleeWeaponDataStruct m_weapon_struct;
		m_weapon_struct.id = melee_weapon->id;
		m_weapon_struct.name = melee_weapon->name;
		m_weapon_struct.type = UAZUtility::StringToEnum<EWeaponType>(melee_weapon->type);
		m_weapon_struct.damage = melee_weapon->damage;
		m_weapon_struct.hone = melee_weapon->hone;
		m_weapon_struct.critical = melee_weapon->critical;
		melee_weapon_data_map_.Emplace(m_weapon_struct.id, m_weapon_struct);
	
	}
	for(auto range_weapon : instance_->table_mgr->range_weapon_array_)
	{
		FRangeWeaponDataStruct r_weapon_struct;
		r_weapon_struct.id = range_weapon->id;
		r_weapon_struct.name = range_weapon->name;
		r_weapon_struct.type = UAZUtility::StringToEnum<EWeaponType>(range_weapon->type);
		r_weapon_struct.damage = range_weapon->damage;
		r_weapon_struct.bottle_array = UAZUtility::StringArrToEnumArr<EBottleType>(range_weapon->ammo_type);
		r_weapon_struct.critical = range_weapon->critical;
		range_weapon_data_map_.Emplace(r_weapon_struct.id, r_weapon_struct);
	}
	for(auto armor : instance_->table_mgr->armor_array_)
	{
		FArmorDataStruct armor_struct;
		armor_struct.id = armor->id;
		armor_struct.type = UAZUtility::StringToEnum<EArmorType>(armor->type);
		armor_struct.defense = armor->defense;
		armor_data_map_.Emplace(armor_struct.id, armor_struct);
	}
}

void UAZInventoryManager::CreateStartItem()
{
	for(auto item : total_data_map_)
	{
		if(item.Value.type == EItemType::potion)
		{
			FPotionInfo info;
			info.item_key = item.Value.id;
			info.item_name = item.Value.name;
			info.item_type = item.Value.type;
			info.item_count = item.Value.init_count;
			FPotionDataStruct* potion_data =  potion_data_map_.Find(item.Value.id);
			info.usable = potion_data->is_usable;
			if(potion_data->is_buff == true)
			{
				FBuffDataStruct* buff_data = buff_data_map_.Find(info.item_key);
				if(buff_data == nullptr)
				{
					return;
				}
				info.item_effect = buff_data->effect;
				info.calc_type = buff_data->calc;
				info.target = buff_data->target;
			}
			AddPocketPotion(info);
		}
		else if(item.Value.type == EItemType::ammo)
		{
			FAmmoInfo info;
			info.item_key = item.Value.id;
			info.item_name = item.Value.name;
			info.item_type = item.Value.type;
			info.item_count = item.Value.init_count;
			FBottleDataStruct* bottle_data = bottle_data_map_.Find(info.item_key);
			if(bottle_data->is_buff == true)
			{
				FBuffDataStruct* buff_data = buff_data_map_.Find(info.item_key);
				info.item_effect = buff_data->effect;
				info.calc_type = buff_data->calc;
				info.effect_type = buff_data->target;
			}
			AddPocketBottle(info);
		}
	}
}
int32 UAZInventoryManager::GetInventoryCurrCount(EItemType item_type, EStorageType type)
{
	int32 count = 0;
	switch (item_type)
	{ 
	case EItemType::potion:
		if (type == EStorageType::Warehouse)
		{
			count = potion_warehouse_.Num();
		}
		else
		{
			count = potion_pocket_.Num();
		}
		break;
	case EItemType::ammo:
		if (type == EStorageType::Warehouse)
		{
			count = bottle_warehouse_.Num();
		}
		else
		{
			count = bottle_pocket_.Num();
		}
		break;
	}
	return count;
}

bool UAZInventoryManager::IsWarehouseFull(EItemType type)
{
	int32 curr_count = 0;
	int32 max_count = 0;
	int32 empty = 0;
	switch (type)
	{
	case EItemType::potion:
		curr_count = GetInventoryCurrCount(EItemType::potion, EStorageType::Warehouse);
		max_count = potion_warehouse_max_count;
		break;
	case EItemType::ammo:
		curr_count = GetInventoryCurrCount(EItemType::ammo, EStorageType::Warehouse);
		max_count = bottle_warehouse_max_count;
		break;
	}
	empty = max_count - curr_count;

	if (empty <= 0)
	{
		return true;
	}
	return false;
}

bool UAZInventoryManager::IsPocketFull(EItemType type)
{
	int32 curr_count = 0;
	int32 max_count = 0;
	int32 empty = 0;
	switch (type)
	{
	case EItemType::potion:
		curr_count = GetInventoryCurrCount(EItemType::potion, EStorageType::Pocket);
		max_count = potion_pocket_max_count;
		break;
	case EItemType::ammo:
		curr_count = GetInventoryCurrCount(EItemType::ammo, EStorageType::Pocket);
		max_count = bottle_pocket_max_count;
		break;
	}
	empty = max_count - curr_count;
	if (empty <= 0)
	{
		return true;
	}
	return false;
}

void UAZInventoryManager::SortWarehouse()
{
	potion_warehouse_.KeySort([](int32 left, int32 right)
	{
		return left < right;
	});
	bottle_warehouse_.KeySort([](int32 left, int32 right)
	{
		return left<right;
	});
}

void UAZInventoryManager::SortPocket()
{
	potion_pocket_.KeySort([](int32 left, int32 right)
	{
		return left<right;
	});
	bottle_pocket_.KeySort([](int32 left, int32 right)
	{
		return left<right;
	});
}


void UAZInventoryManager::RemoveItem(int32 item_key, EItemType item_type, EStorageType type)
{
	switch (item_type)
	{
	case EItemType::potion:
		if(type == EStorageType::Warehouse)
		{
			potion_warehouse_.Remove(item_key);
		}
		else if(type == EStorageType::Pocket)
		{
			potion_pocket_.Remove(item_key);
		}
		break;
	case EItemType::ammo:
		if(type == EStorageType::Warehouse)
		{
			bottle_warehouse_.Remove(item_key);
		}
		else if(type == EStorageType::Pocket)
		{
			bottle_pocket_.Remove(item_key);
		}
		break;
	}
}

TArray<UAZPotionItem*> UAZInventoryManager::GetPotionSlot()
{
	return potion_slot_;
}


TArray<UAZAmmoItem*> UAZInventoryManager::GetBottleSlot()
{
	return bottle_slot_;
}

bool UAZInventoryManager::AddWarehousePotion(FPotionInfo& info)
{
	bool is_full = IsWarehouseFull(EItemType::potion);
	if (is_full == true)
	{
		return false;
	}

	UAZPotionItem** potion = potion_warehouse_.Find(info.item_key);

	if(potion!= nullptr)
	{
		(*potion)->IncreaseCount(info.item_count);
	}
	else
	{
		info.storage_type = EStorageType::Warehouse;
		potion_warehouse_.Emplace(info.item_key,CreatePotion(info));
	}
	return true;
}

bool UAZInventoryManager::AddPocketPotion( FPotionInfo& info)
{
	bool is_full = IsPocketFull(EItemType::potion);
	if (is_full == true)
	{
		return false;
	}
	UAZPotionItem** potion = potion_pocket_.Find(info.item_key);
	if(potion != nullptr)
	{
		(*potion)->IncreaseCount(info.item_count);
	}
	else
	{
		info.storage_type = EStorageType::Pocket;
		potion_pocket_.Emplace(info.item_key, CreatePotion(info));
	}
	
	return true;
}

UAZPotionItem* UAZInventoryManager::CreatePotion(FPotionInfo& info)
{
	UAZPotionItem* potion = NewObject<UAZPotionItem>();
	potion->InitItem(info);
	return potion;
}


bool UAZInventoryManager::ChangePotionStorage(int32 item_key, EStorageType type, int32 move_count)
{
	EStorageType pre_state = type;
	FPotionInfo new_info;
	UAZPotionItem** potion = nullptr;
	if(pre_state == EStorageType::Warehouse)
	{
		potion =  potion_warehouse_.Find(item_key);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		potion = potion_pocket_.Find(item_key);
	}
	
	if(potion == nullptr)
	{
		return false;
	}
	FPotionInfo info = (*potion)->GetItemInfo();
	info.item_count -= move_count;
	if(info.item_count<=0)
	{
		RemoveItem(info.item_key,info.item_type, info.storage_type);
	}
	new_info = info;
	new_info.item_count = move_count;
	if(pre_state == EStorageType::Warehouse)
	{
		AddPocketPotion(new_info);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		AddWarehousePotion(new_info);
	}
	return true;
}

void UAZInventoryManager::SetPotionSlot()
{
	for(auto item : potion_pocket_)
	{
		potion_slot_.Emplace(item.Value);
	}
}

void UAZInventoryManager::ResetPotionSlot()
{
	potion_slot_.Reset();
}

	FBuffDataStruct UAZInventoryManager::UsePotion(int32 item_index)
{
	UAZPotionItem* potion = potion_slot_[item_index];
	FBuffDataStruct buff;
	
	potion->DecreaseCount();
	if(potion->GetItemCount() == 0)
	{
		potion_pocket_.Remove(potion->GetItemInfo().item_key);
		potion_slot_.Remove(potion);
	}
	buff = *buff_data_map_.Find(potion->GetItemInfo().item_key);
	UAZPotionItem** temp =  potion_pocket_.Find(buff.id);
	if(temp != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  FString::Printf(TEXT("item : %s , count : %d"),
			*((*temp)->GetItemInfo().item_name),(*temp)->GetItemInfo().item_count));
	}
	return buff;
}

bool UAZInventoryManager::AddWarehouseBottle(FAmmoInfo& info)
{
	bool is_full = IsWarehouseFull(EItemType::ammo);
	if(is_full == true)
	{
		return false;
	}
	UAZAmmoItem** bottle = bottle_warehouse_.Find(info.item_key);

	if(bottle != nullptr)
	{
		(*bottle)->IncreaseCount();
	}
	else
	{
		info.storage_type = EStorageType::Warehouse;
		bottle_warehouse_.Emplace(info.item_key, CreateBottle(info));
	}
	return true;
}

bool UAZInventoryManager::AddPocketBottle(FAmmoInfo& info)
{
	bool is_full = IsPocketFull(EItemType::ammo);
	if(is_full == true)
	{
		return false;
	}
	UAZAmmoItem** bottle = bottle_pocket_.Find(info.item_key);
	if(bottle != nullptr)
	{
		(*bottle)->IncreaseCount();
	}
	else
	{
		info.storage_type = EStorageType::Pocket;
		bottle_pocket_.Emplace(info.item_key,CreateBottle(info));
	}
	return true;
}

UAZAmmoItem* UAZInventoryManager::CreateBottle(FAmmoInfo& info)
{
	UAZAmmoItem* bottle = NewObject<UAZAmmoItem>();
	bottle->InitItem(info);
	return bottle;
}

UAZWeaponItem* UAZInventoryManager::CreateWeapon(FWeaponInfo& info)
{
	UAZWeaponItem* weapon = NewObject<UAZWeaponItem>();
	weapon->InitItem(info);
	return weapon;
}

bool UAZInventoryManager::AddWarehouseWeapon(FWeaponInfo& info)
{
	UAZWeaponItem** weapon = weapon_warehouse_.Find(info.item_key);

	if(weapon != nullptr)
	{
		return false;
	}
	else
	{
		weapon_warehouse_.Emplace(info.item_key, CreateWeapon(info));
	}

	return true;
}

void UAZInventoryManager::EquipWeapon(int32 item_key)
{
	UAZWeaponItem** weapon = weapon_warehouse_.Find(item_key);
	if(weapon == nullptr)
	{
		return;
	}
	if((*weapon)->GetEquipState() == false)
	{
		(*weapon)->EquipStateChange(true);
	}
	else if((*weapon)->GetEquipState() == true)
	{
		(*weapon)->EquipStateChange(false);
	}
}

UAZWeaponItem* UAZInventoryManager::GetEquipWeapon()
{
	UAZWeaponItem* equip_weapon = nullptr;
	for(auto weapon : weapon_warehouse_)
	{
		if(weapon.Value->GetEquipState() == true)
		{
			equip_weapon = weapon.Value;
		}
	}
	return equip_weapon;
}

UAZArmorItem* UAZInventoryManager::CreateArmor(FArmorInfo& info)
{
	UAZArmorItem* armor = NewObject<UAZArmorItem>();
	armor->InitItem(info);
	return armor;
}

bool UAZInventoryManager::AddWarehouseArmor(FArmorInfo& info)
{
	UAZArmorItem** armor = armor_warehouse_.Find(info.item_key);

	if(armor != nullptr)
	{
		return false;
	}
	armor_warehouse_.Emplace(info.item_key, CreateArmor(info));
	return true;
}

void UAZInventoryManager::EquipArmor(int32 item_key)
{
	UAZArmorItem** armor = armor_warehouse_.Find(item_key);
	if(armor == nullptr)
	{
		return;
	}
	if((*armor)->GetEquipState() == false)
	{
		(*armor)->EquipStateChange(true);
	}
	else if((*armor)->GetEquipState() == true)
	{
		(*armor)->EquipStateChange(false);
	}
}

UAZArmorItem* UAZInventoryManager::GetEquipArmor(int32 ui_index)
{
	return nullptr;
}

bool UAZInventoryManager::ChangeBottleStorage(int32 item_key, EStorageType type, int32 move_count)
{
	EStorageType pre_state = type;
	FAmmoInfo new_info;
	UAZAmmoItem** bottle = nullptr;
	if(pre_state == EStorageType::Warehouse)
	{
		bottle = bottle_warehouse_.Find(item_key);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		bottle = bottle_pocket_.Find(item_key);
	}
	if(bottle == nullptr)
	{
		return false;
	}
	FAmmoInfo info = (*bottle)->GetItemInfo();
	new_info = info;
	(*bottle)->DecreaseCount(move_count);
	if((*bottle)->GetItemCount() == 0)
	{
		RemoveItem(info.item_key, info.item_type, info.storage_type);
	}
	new_info.item_count = move_count;
	if(pre_state ==EStorageType::Warehouse)
	{
		AddPocketBottle(new_info);
	}
	else if(pre_state == EStorageType::Pocket)
	{
		AddWarehouseBottle(new_info);
	}
	return true;
}

void UAZInventoryManager::SetBottleSlot()
{
	for(auto item : bottle_pocket_)
	{
		bottle_slot_.Emplace(item.Value);
	}
}

FBuffDataStruct UAZInventoryManager::UseBottle(int32 index)
{
	FBuffDataStruct buff;
	if(index+1 >bottle_slot_.Num())
	{
		return buff;
	}
	UAZAmmoItem* item =bottle_slot_[index];
	

	if(item->GetItemInfo().item_count != 99)
	{
		item->DecreaseCount();
	}
	
	if(item->GetItemCount() == 0)
	{
		bottle_pocket_.Remove(item->GetItemKey());
		bottle_slot_.Remove(item);
	}
	buff = *buff_data_map_.Find(item->GetItemKey());
	UAZAmmoItem** temp =  bottle_pocket_.Find(buff.id);
	if(temp != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red,  FString::Printf(TEXT("item : %s , count : %d"),
			*((*temp)->GetItemInfo().item_name),(*temp)->GetItemInfo().item_count));
	}
	return buff;
}