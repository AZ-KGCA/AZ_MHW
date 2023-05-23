// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Item/AZPotionItem.h"
#include "AZ_MHW/Item/AZAmmoItem.h"
#include "AZ_MHW/Item/AZWeaponItem.h"
#include "AZ_MHW/Item/AZArmorItem.h"
#include "UObject/NoExportTypes.h"
#include "AZInventoryManager.generated.h"

/**
 * 
 */


UCLASS(Blueprintable,BlueprintType)
class AZ_MHW_API UAZInventoryManager : public UObject
{
	GENERATED_BODY()
private:
	UPROPERTY() TMap<int32, FTotalItemDataStruct> total_data_map_;
	UPROPERTY() TMap<int32, FPotionDataStruct> potion_data_map_;
	UPROPERTY() TMap<int32, FBuffDataStruct> buff_data_map_;
	UPROPERTY() TMap<int32, FBottleDataStruct> bottle_data_map_;
public:
	UPROPERTY() TMap<int32, UAZPotionItem*> potion_pocket_;
	UPROPERTY() TMap<int32, UAZPotionItem*> potion_warehouse_;
	UPROPERTY() TMap<int32, UAZAmmoItem*> bottle_pocket_;
	UPROPERTY() TMap<int32, UAZAmmoItem*> bottle_warehouse_;
	UPROPERTY() TMap<int32, UAZWeaponItem*> weapon_warehouse_;
	UPROPERTY() TMap<int32, UAZArmorItem*> armor_warehouse_;
	UPROPERTY() class UAZGameSingleton* instance_;

	//UI 
	UPROPERTY() TArray<UAZPotionItem*> potion_slot_;
	UPROPERTY() TArray<UAZAmmoItem*> bottle_slot_;
	
	
	void					Init();
	void					ResetMgr();
	void					SetMaxCount();
	void					GetTableData();
	int32					GetInventoryCurrCount(EItemType item_type, EStorageType type);
	bool					IsWarehouseFull(EItemType type);
	bool					IsPocketFull(EItemType type);
	void					SortWarehouse();
	void					SortPocket();
	void					CreateStartItem();
	void					RemoveItem(int32 item_key, EItemType item_type,EStorageType type);
	TArray<UAZPotionItem*>	GetPotionSlot();
	TArray<UAZAmmoItem*>	GetBottleSlot();
	
	//potion
	bool				AddWarehousePotion(FPotionInfo& info);
	bool				AddPocketPotion(FPotionInfo& info);
	bool				ChangePotionStorage(int32 item_key, EStorageType type, int32 move_count);
	void				SetPotionSlot();
	FBuffDataStruct		UsePotion(int32 index);
	UAZPotionItem*		CreatePotion(FPotionInfo& info);

	//Ammo
	bool				AddWarehouseBottle(FAmmoInfo& info);
	bool				AddPocketBottle(FAmmoInfo& info);
	bool				ChangeBottleStorage(int32 item_key, EStorageType type, int32 move_count);
	void				SetBottleSlot();
	FBuffDataStruct		UseBottle(int32 index);
	UAZAmmoItem*		CreateBottle(FAmmoInfo& info);
	
	//weapon
	UAZWeaponItem*		CreateWeapon(FWeaponInfo& info);
	bool				AddWarehouseWeapon(FWeaponInfo& info);
	void				EquipWeapon(int32 item_key);
	
	//UAZPotionItem*	GetPotionItem(FPotionInfo& info);
	/*
	bool		AddWarehouseItem(const FItemInfo& item_info);
	bool		AddPocketItem(const FItemInfo& item_info);
	bool		RemoveItem(const FItemInfo& item_info);
	bool		ChangeItem(const FItemInfo& item_info, EStorageType type, int32  move_count = 1);
	bool		UseItem(int32 pocket_index);
	bool		IsInventoryFUll(EStorageType type);
	void		SortInventoryByIndex();
	void		InitInventoryCount();
	void		RemoveAll(int32 item_index, EStorageType type);
	void		CreateDefaultItem();
	UAZPotionItem*	FindItem(const FItemInfo& item_info);
	UAZPotionItem*	InitializeItem(const FItemInfo &ItemInfo);
	int32		GetInventoryMaxCount(EStorageType type);
	int32		GetInventoryCurCount(EStorageType type);
	*/
private:
	int32 potion_warehouse_max_count;
	int32 potion_pocket_max_count;
	int32 bottle_warehouse_max_count;
	int32 bottle_pocket_max_count;
};