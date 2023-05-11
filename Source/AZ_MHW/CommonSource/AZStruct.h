#pragma once
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "AZStruct.generated.h"

//============================================
// Character Common

USTRUCT(BlueprintType)
struct FStatusEffectInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) EStatusEffectType type;
	UPROPERTY(EditAnywhere) float duration;

	FStatusEffectInfo() : type(EStatusEffectType::None), duration(0) {};
	FStatusEffectInfo(EStatusEffectType type, float duration) : type(type), duration(duration) {};
};

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere) int32 base_damage;
	UPROPERTY(EditAnywhere) EDamageType damage_type;
	UPROPERTY(EditAnywhere) TArray<FStatusEffectInfo> status_effects;

	FAttackInfo() : base_damage(0), damage_type(EDamageType::None) {};
	FAttackInfo(int32 base_damage, EDamageType damage_type) : base_damage(base_damage), damage_type(damage_type) {};
	FAttackInfo(int32 base_damage, EDamageType damage_type, TArray<FStatusEffectInfo> status_effects) :
		base_damage(base_damage), damage_type(damage_type), status_effects(status_effects) {};
};

// End of Character Common
//============================================
// Monster

USTRUCT(BlueprintType)
struct FMonsterActionStateInfo
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EMonsterActionMode action_mode;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EMonsterActionPriority priority_score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EMoveState move_state;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FName animation_name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) FName montage_section_name;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float target_angle;

	FMonsterActionStateInfo()
	{
		action_mode = EMonsterActionMode::Normal;
		priority_score = EMonsterActionPriority::None;
		move_state = EMoveState::None;
		animation_name = montage_section_name = NAME_None;
		target_angle = 0.0f;
	}
	void Reset()
	{
		action_mode = EMonsterActionMode::Normal;
		priority_score = EMonsterActionPriority::Locomotion;
		move_state = EMoveState::None;
		animation_name = NAME_None;
		montage_section_name = NAME_None;
		target_angle = 0.0f;
	}
};

USTRUCT(BlueprintType)
struct FBossRageStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 required_damage;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float duration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float outgoing_damage_multiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float incoming_damage_multiplier;

	FBossRageStats()
	{
		required_damage = duration = outgoing_damage_multiplier = incoming_damage_multiplier = 0.0f;
	};
	FBossRageStats(const int32 required_damage, const int32 duration, const int32 outgoing_damage_multiplier, const int32 incoming_damage_multiplier)
	{
		this->required_damage = required_damage;
		this->duration = UAZUtility::MillisecondsToSeconds(duration);
		this->outgoing_damage_multiplier = UAZUtility::PerTenThousandToPerOne(outgoing_damage_multiplier);
		this->incoming_damage_multiplier = UAZUtility::PerTenThousandToPerOne(incoming_damage_multiplier);
	}
	FBossRageStats(const int32 required_damage, const float duration, const float outgoing_damage_multiplier, const float incoming_damage_multiplier)
	{
		this->required_damage = required_damage;
		this->duration = duration;
		this->outgoing_damage_multiplier = outgoing_damage_multiplier;
		this->incoming_damage_multiplier = incoming_damage_multiplier;
	}
};

USTRUCT(BlueprintType)
struct FBossEscapeStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 num_allowed_escapes;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<float> escape_health_ratios;

	FBossEscapeStats()
	{
		num_allowed_escapes = 0;
		escape_health_ratios.Empty();
	};
	FBossEscapeStats(const int32 num_allowed_escapes, const TArray<float>& escape_health_ratios)
	{
		this->num_allowed_escapes = num_allowed_escapes;
		this->escape_health_ratios = escape_health_ratios;
	}
	FBossEscapeStats(const int32 num_total_escapes, const TArray<int32>& escape_health_ratios)
	{
		this->num_allowed_escapes = num_allowed_escapes;
		for (const int32 escape_health_ratio : escape_health_ratios)
		{
			this->escape_health_ratios.Add(UAZUtility::PerTenThousandToPerOne(escape_health_ratio));
		}
	}
};

USTRUCT(BlueprintType)
struct FBossBodyPartDebuffInfo
{
	GENERATED_BODY()

	// Wound 	
	bool is_woundable;
	int32 wound_required_damage;
	int32 wound_accumulated_damage;
	
	// Part break 
	bool is_breakable;
	int32 break_required_damage;
	int32 break_accumulated_damage;

	// Sever 
	bool is_severable;
	int32 sever_required_damage;
	int32 sever_accumulated_damage;
	
	bool is_stunnable;

	FBossBodyPartDebuffInfo()
	{
		is_breakable = is_severable = is_woundable = is_stunnable = false;
		break_required_damage = break_accumulated_damage = 0;
		sever_required_damage = sever_accumulated_damage = 0;
		wound_required_damage = wound_accumulated_damage = 0;
	}
	FBossBodyPartDebuffInfo(TArray<int32> damage_info) : FBossBodyPartDebuffInfo()
	{
		// Wound
		if (damage_info[0] != -1)
		{
			is_woundable = true;
			wound_required_damage = damage_info[2];
		}
		// Break
		if (damage_info[1] != -1)
		{
			is_breakable = true;
			break_required_damage = damage_info[0];
		}
		// Sever
		if (damage_info[2] != -1)
		{
			is_severable = true;
			sever_required_damage = damage_info[1];
		}
	}
};

USTRUCT(BlueprintType)
struct FMonsterSightConfigs
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 radius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 lose_radius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float fov;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float max_age;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 auto_success_range;

	FMonsterSightConfigs()
	{
		radius = 1000;
		lose_radius = 1200;
		fov = 70.0f;
		max_age = 3.0f;
		auto_success_range = 300.0f;
	};
	FMonsterSightConfigs(int32 radius, int32 lose_radius, int32 fov, float max_age, int32 auto_success_range)
	{
		this->radius = radius;
		this->lose_radius = lose_radius;
		this->fov = fov;
		this->max_age = max_age;
		this->auto_success_range = auto_success_range;
	}
};

USTRUCT(BlueprintType)
struct FBossWeaknessStats
{
	GENERATED_BODY()

	TArray<int32> head;
	TArray<int32> neck;
	TArray<int32> body;
	TArray<int32> tail;
	TArray<int32> wing;
	TArray<int32> leg;
	TArray<TArray<int32>> weakness_array;
	TMap<EMonsterBodyPart, TArray<int32>*> weakness_map;
	
	FBossWeaknessStats() {};
	FBossWeaknessStats(const TArray<int32>& head_weakness, const TArray<int32>& neck_weakness,
		const TArray<int32>& body_weakness,	const TArray<int32>& tail_weakness,
		const TArray<int32>& wing_weakness, const TArray<int32>& leg_weakness)
	{
		// cut - blunt - ammo
		head = head_weakness;
		neck = neck_weakness;
		body = body_weakness;
		tail = tail_weakness;
		wing = wing_weakness;
		leg  = leg_weakness;

		TArray<int32> empty_array;
		weakness_array = { empty_array, head_weakness, wing_weakness, wing_weakness,
			tail_weakness, neck_weakness, body_weakness, body_weakness, leg_weakness };

		weakness_map.Add(EMonsterBodyPart::Head, &head);
		weakness_map.Add(EMonsterBodyPart::RightWing, &wing);
		weakness_map.Add(EMonsterBodyPart::LeftWing, &wing);
		weakness_map.Add(EMonsterBodyPart::Tail, &tail);
		weakness_map.Add(EMonsterBodyPart::Neck, &neck);
		weakness_map.Add(EMonsterBodyPart::Body, &body);
		weakness_map.Add(EMonsterBodyPart::Back, &body);
		weakness_map.Add(EMonsterBodyPart::Leg, &leg);
	}
};

struct FMonsterInfo
{
	int32 monster_id;
	EMonsterBehaviorType behavior_type;
	int32 base_hp;
	FMonsterSightConfigs sight_configs;
	int32 patrol_range;
	float patrol_delay;
	int32 percept_radius;
	FName behavior_tree_filename;

	FMonsterInfo()
	{
		monster_id = -1;
		behavior_type = EMonsterBehaviorType::Neutral;
		base_hp = 1000;
		sight_configs = FMonsterSightConfigs();
		patrol_range = 1000;
		patrol_delay = 5.0f;
		percept_radius = 1500;
		behavior_tree_filename = NAME_None;
	}
};

struct FBossInfo
{
	int32 boss_id;
	int32 monster_id;
	EBossRank rank;
	int32 base_stamina;
	float tired_duration;
	bool has_transition_animation;
	FBossEscapeStats escape_stats;
	FBossWeaknessStats weakness_stats;
	FBossBodyPartDebuffInfo head_state;
	FBossBodyPartDebuffInfo body_state;
	FBossBodyPartDebuffInfo wing_state;
	FBossBodyPartDebuffInfo tail_state;
	FBossBodyPartDebuffInfo leg_state;
	TArray<EMonsterBodyPart> stunnable_parts;
	FBossRageStats rage_stats;
	float tenderised_damage_multiplier;
};

USTRUCT(BlueprintType)
struct FMonsterCombatActionInfo
{
	GENERATED_BODY()
	
	int32 action_id;
	int32 monster_id;
	FName animation_name;
	FName montage_section_name;
	EAttackEffect attack_effect;
	int32 damage_base;
	int32 damage_poison;
	EMonsterActionTriggerType triggers;
	EMonsterActionConditionType conditions;
	int32 condition_min_distance_from_target;
	int32 condition_max_distance_from_target;
	float cooltime;
	float cooltime_timer;

	FMonsterCombatActionInfo() {}
};

USTRUCT(BlueprintType)
struct FMonsterNonCombatActionInfo
{
	GENERATED_BODY()
	
	int32 action_id;
	int32 monster_id;
	FName animation_name;
	FName montage_section_name;
	EMonsterActionConditionType conditions;
	float condition_min_health_ratio;		// only checked if condition includes Health
	float condition_max_health_ratio;

	FMonsterNonCombatActionInfo() {}
};

USTRUCT(BlueprintType)
struct FBossBodyCondition
{
	GENERATED_BODY()
	
	//TODO states of body parts (e.g. broken, damaged, etc)

	void Reset() {}
};


// End of Monster
//============================================