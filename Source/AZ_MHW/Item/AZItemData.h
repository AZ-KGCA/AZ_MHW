#pragma once
#include "AZItemData.generated.h"



UENUM(BlueprintType)
enum class EItemType :uint8
{
	Info = 0,
	Potion,
	Bottle,
	Weapon,
	Armor,
	
};
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WeaponFar,
	WeaponMelee,
};

UENUM(BlueprintType)
enum class EStorageType : uint8
{
	None,
	Warehouse,
	Pocket,
};

UENUM(BlueprintType)
enum class EPotionTarget :uint8
{
	Health,
	Damage,
};

UENUM(BlueprintType)
enum class EBottleEffect : uint8
{
	None,
	Damage,
};

UENUM(BlueprintType)
enum class ECalculation :uint8
{
	Plus,
	Multi,
};

UENUM(BlueprintType)
enum class EArmorType : uint8
{
	Helmet,
	Chest,
	Arm,
	Belt,
	Leg,
};

UENUM(BlueprintType)
enum class EBottleType : uint8
{
	None,
	NormalBottle,//빈 병
	PowerBottle,// 데미지 업
};

/*
UENUM(BlueprintType)
enum class EItemEffect :uint8
{
	PotionEffect,
};

USTRUCT(BlueprintType)
struct FTotalItemInfo
{	
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)	int32 item_index;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)	FString item_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString desc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 warehouse_max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 pocket_max_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EItemType type;

	FTotalItemInfo()
	{
		item_index = 0;
		item_name = "none";
		count = 0;
		desc = "none";
		warehouse_max_count = 0;
		pocket_max_count = 0;
		
	}

};*/

USTRUCT(BlueprintType)
struct FItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 item_key;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FString item_name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EItemType item_type;
	/*int32 item_count;
	int32 item_index;
	EStorageType storage_type;
	FString item_name;*/
	//effect name
	//item_icon
	//item_particle
	//model_name
	FItemInfo()
	{
		item_key = 0;
		item_name = "none";
		item_type = EItemType::Potion;

	}
};

USTRUCT(BlueprintType)
struct FPotionInfo : public FItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EPotionTarget target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 item_effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 item_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool usable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EStorageType storage_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ECalculation calc_type;
	
	FPotionInfo()
	{
		item_key = 101;
		item_name = "none";
		target = EPotionTarget::Health;
		item_effect = 0;
		usable = true;
		item_count = 0;
		calc_type = ECalculation::Plus;
		storage_type = EStorageType::Warehouse;
	}
};

USTRUCT(BlueprintType)
struct FAmmoInfo : public FItemInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 item_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EBottleEffect effect_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 item_effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EStorageType storage_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ECalculation calc_type;

	FAmmoInfo()
	{
		item_key = 0;
		item_name = "none";
		item_count = 0;
		effect_type = EBottleEffect::None;
		
		item_effect = 0;
		calc_type = ECalculation::Multi;
	}
};

USTRUCT(BlueprintType)
struct FWeaponInfo : public FItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 damage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EWeaponType weapon_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<EBottleType> bottle_type_array;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 bottle_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool is_equip;
	FWeaponInfo()
	{
		item_key = 0;
		item_name = "none";
		damage = 0;
		weapon_type  = EWeaponType::WeaponMelee;
		bottle_count = 0;
		is_equip = false;
	}
};


USTRUCT(BlueprintType)
struct FArmorInfo : public FItemInfo 
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite) EArmorType armor_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 defense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool is_equip;

	FArmorInfo()
	{
		armor_type = EArmorType::Helmet;
		defense = 10;
		is_equip = false;
	}
};