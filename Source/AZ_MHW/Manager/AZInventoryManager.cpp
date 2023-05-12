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

	UsePotion(0);
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
		if(total_item->count == 1)
		{
			FTotalItemDataStruct total_data;
			total_data.id = total_item->id;
			total_data.name = total_item->name;
			total_data.warehouse_max = total_item->warehouse_max;
			total_data.pocket_max = total_item->pocket_max;
			total_data.init_count = total_item->init_count;
			FString type = total_item->type;
			total_data.type = UAZUtility::StringToEnum<EItemType>(type);
			total_data_map_.Emplace(total_data.id, total_data);
		}
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

FBuffDataStruct UAZInventoryManager::UsePotion(int32 item_index)
{
	UAZPotionItem* potion = potion_slot_[item_index];
	FBuffDataStruct buff;
	potion->DecreaseCount();
	if(potion->GetItemCount() == 0)
	{
		potion_pocket_.Remove(potion->GetItemInfo().item_key);
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
	UAZAmmoItem* item =bottle_slot_[index];
	FBuffDataStruct buff;

	if(item->GetItemInfo().item_name != "normal_bottle")
	{
		item->DecreaseCount();
	}
	
	if(item->GetItemCount() == 0)
	{
		bottle_pocket_.Remove(item->GetItemKey());
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

/*
void UAZItemInventoryManager::InitInventoryCount()
{
	warehouse_max_count = 50;
	pocket_max_count = 15;
}

void UAZItemInventoryManager::RemoveAll(int32 item_index, EStorageType type)
{
	/*if (type == EStorageType::Warehouse)
	{
		warehouse_map.Remove(item_index);
	}
	else if (type == EStorageType::Pocket)
	{
		pocket_map.Remove(item_index);
	}#1#
}

void UAZItemInventoryManager::CreateDefaultItem()
{
	/*FItemInfo info;
	for(int i = 0 ; i<instance->table_mgr->total_item_array.Num(); i++)
	{
		info.item_key = instance->table_mgr->total_item_array[i]->id;
		info.item_name = instance->table_mgr->total_item_array[i]->name;
		info.storage_type = EStorageType::Pocket;
		info.item_count = instance->table_mgr->total_item_array[i]->init_count;
		std::string str = TCHAR_TO_UTF8(*instance->table_mgr->total_item_array[i]->type);
		info.item_type = magic_enum::enum_cast<EItemType>(str).value();
		AddPocketItem(info);
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), pocket_map.Num());#1#
}

bool UAZItemInventoryManager::AddWarehouseItem(const FItemInfo& item_info)
{
	/*FItemInfo new_info = item_info;
	bool is_full = IsInventoryFUll(EStorageType::Warehouse);
	if (is_full == true)
	{
		return false;
	}
	UAZItem** item = warehouse_map.Find(new_info.item_key);
	if (item != nullptr)
	{
		(*item)->IncreaseCount();
	} 
	else
	{
		warehouse_map.Emplace(new_info.item_key, InitializeItem(new_info));
	}
	return true;#1#
}

bool UAZItemInventoryManager::AddPocketItem(const FItemInfo& item_info)
{
	/*FItemInfo info = item_info;
	bool is_full = IsInventoryFUll(EStorageType::Pocket);
	if (is_full == true)
	{
		bool check_warehouse = IsInventoryFUll(EStorageType::Warehouse);
		if (check_warehouse == true)
		{
			//아이템 제거
			return false;
		}
		UAZItem** item = warehouse_map.Find(info.item_key);
		if (item != nullptr)
		{
			(*item)->IncreaseCount();
		}
		else
		{
			warehouse_map.Emplace(info.item_key, InitializeItem(info));
		}
	}
	else
	{
		UAZItem** item = pocket_map.Find(info.item_key);
		if (item != nullptr)
		{
			(*item)->IncreaseCount();
		}
		else
		{
			pocket_map.Emplace(info.item_key, InitializeItem(info));
		}
	}#1#
	return true;
}

bool UAZItemInventoryManager::RemoveItem(const FItemInfo& item_info)
{
	/*FItemInfo info = item_info;
	UAZItem* item = FindItem(info);

	if (item != nullptr)
	{
		item->DecreaseCount();
		if (item->GetItemCount() <= 0)
		{
			RemoveAll(item->GetItemIndex(), item->GetItemStorageType());
		}
	}#1#
	return true;
}

bool UAZItemInventoryManager::ChangeItem(const FItemInfo& item_info, EStorageType type, int32 move_count)
{
	/*FItemInfo info = item_info;
	EStorageType before_state = type;
	if (before_state == EStorageType::Warehouse)
	{
		if (warehouse_map.Contains(info.item_key))
		{
			info.item_count-=move_count;

			if (info.item_count <= 0)
			{
				warehouse_map.Remove(info.item_key);
				info.item_count = move_count;
				info.storage_type = EStorageType::Pocket;
			}
			else
			{
				info.item_count = move_count;
				info.storage_type = EStorageType::Pocket;
			}
			AddPocketItem(info);
		}
	}
	else if (before_state == EStorageType::Pocket)
	{
		if (pocket_map.Contains(info.item_key))
		{
			info.item_count--;
			if (info.item_count <= 0)
			{
				pocket_map.Remove(info.item_key);
				info.item_count = move_count;
				info.storage_type = EStorageType::Warehouse;
			}
			else
			{
				info.item_count = move_count;
				info.storage_type = EStorageType::Pocket;
			}
			AddWarehouseItem(info);
		}
	}#1#
	return true;
}

bool UAZItemInventoryManager::UseItem(int32 pocket_index)
{
	/*UAZItem* use_item = pocket_map[pocket_index];
	use_item->DecreaseCount();
	if (use_item->GetItemCount() <= 0)
	{
		pocket_map.Remove(use_item->GetItemIndex());
	}#1#
	return true;
}


bool UAZItemInventoryManager::IsInventoryFUll(EStorageType type)
{
	/*int32 max_count = GetInventoryMaxCount(type);
	int32 cur_count = GetInventoryCurCount(type);
	if (max_count == cur_count)
	{
		return true;
		
	}#1#
	return false;
}

void UAZItemInventoryManager::SortInventoryByIndex()
{
	warehouse_map.KeySort([](int32 left, int32 right)
	{
		return left < right;
	});
}

UAZPotionItem* UAZItemInventoryManager::FindItem(const FItemInfo& item_info)
{
	/*FItemInfo info = item_info;
	UAZItem** find_item = nullptr;
	if (info.storage_type == EStorageType::Warehouse)
	{
		find_item = warehouse_map.Find(info.item_key);
	}
	else if (info.storage_type == EStorageType::Pocket)
	{
		find_item = pocket_map.Find(info.item_key);
	}#1#
	return nullptr;
}

UAZPotionItem* UAZItemInventoryManager::InitializeItem(const FItemInfo& ItemInfo)
{
	/*UAZItem* item = NewObject<UAZItem>();
	item->InitItem(ItemInfo);
	return item;#1#
	return nullptr;
}

int32 UAZItemInventoryManager::GetInventoryMaxCount(EStorageType type)
{
	/*if (type == EStorageType::Warehouse)
	{
		return warehouse_max_count;
	}
	else
	{
		return pocket_max_count;
	}#1#
}

int32 UAZItemInventoryManager::GetInventoryCurCount(EStorageType type)
{
	/*if (type == EStorageType::Warehouse)
	{
		return warehouse_map.Num();
	}
	else
	{
		return pocket_map.Num();
		
	}#1#
}
*/
