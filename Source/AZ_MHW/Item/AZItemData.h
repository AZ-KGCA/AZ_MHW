#pragma once
#include "AZItemData.generated.h"



UENUM(BlueprintType)
enum class EItemType :uint8
{
	info,
	potion,
	ammo,
	weapon,
	armor,
	
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
enum class EItemTarget :uint8
{
	None,
	Health,
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
		item_type = EItemType::potion;

	}
};

USTRUCT(BlueprintType)
struct FPotionInfo : public FItemInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EItemTarget target;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) float item_effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 item_count;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) bool usable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EStorageType storage_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ECalculation calc_type;
	
	FPotionInfo()
	{
		item_key = 101;
		item_name = "none";
		target = EItemTarget::Health;
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EItemTarget effect_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 item_effect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) EStorageType storage_type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) ECalculation calc_type;

	FAmmoInfo()
	{
		item_key = 0;
		item_name = "none";
		item_count = 0;
		effect_type = EItemTarget::None;
		
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

USTRUCT()
struct FTotalItemDataStruct
{
	GENERATED_BODY()
	int32 id;
	FString name;
	int32 warehouse_max;
	int32 pocket_max;
	int32  init_count;
	EItemType type;
	FTotalItemDataStruct()
	{
		id = 0;
		name = "none";
		warehouse_max = 0;
		pocket_max = 0;
		init_count = 0;
	}
};

USTRUCT()
struct FPotionDataStruct
{
	GENERATED_BODY()
	int32 id;
	bool is_buff;
	bool is_usable;
	FPotionDataStruct()
	{
		id =0;
		is_buff = false;
		is_usable = false;
	}
};

USTRUCT()
struct FBuffDataStruct
{
	GENERATED_BODY()
	int32 id;
	EItemTarget target;
	ECalculation calc;
	float effect; //TODO : change int
	FBuffDataStruct()
	{
		id = 0;
		target = EItemTarget::None;
		effect = 0.0f;
	}
};

USTRUCT()
struct FBottleDataStruct
{
	GENERATED_BODY()
	int32 id;
	bool is_buff;
	FBottleDataStruct()
	{
		id =0;
		is_buff = false;
	}
};
