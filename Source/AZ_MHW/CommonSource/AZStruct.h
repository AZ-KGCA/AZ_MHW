#pragma once
#include "AITypes.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZStruct.generated.h"


//============================================
// Monster

USTRUCT(BlueprintType)
struct FBossIdentifier
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 monster_id;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) EBossRank rank;

	FBossIdentifier()
	{
		monster_id = -1;
		rank = EBossRank::None;
	};
	FBossIdentifier(int32 monster_id, EBossRank rank)
	{
		this->monster_id = monster_id;
		this->rank = rank;
	}
};
FORCEINLINE uint32 GetTypeHash(const FBossIdentifier& boss_id)
{
	return FCrc::MemCrc32(&boss_id, sizeof(FBossIdentifier));
}

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

struct FMonsterInfo
{
	int32 monster_id;
	EMonsterBehaviorType behavior_type;
	int32 base_hp;
	int32 sight_radius;
	int32 sight_lose_radius;
	int32 sight_fov;
	float sight_max_age;
	int32 sight_auto_success_range;
	int32 patrol_range;
	float patrol_delay;
	int32 percept_radius;
	FName behavior_tree_filename;
};

struct FBossInfo
{
	int32 boss_id;
	int32 monster_id;
	EBossRank rank;
	int32 base_stamina;
	float tired_duration;
	bool has_transition_animation;
	int32 num_allowed_escapes;
	TArray<float> escape_health_ratios;
	TArray<int32> weakness_head;
	TArray<int32> weakness_neck;
	TArray<int32> weakness_body;
	TArray<int32> weakness_tail;
	TArray<int32> weakness_wing;
	TArray<int32> weakness_leg;
	int32 break_damage_head;
	int32 break_damage_body;
	int32 break_damage_wing;
	int32 sever_damage_tail;
	int32 rage_required_damage;
	float rage_duration;
	float rage_outgoing_damage_multiplier;
	float rage_incoming_damage_multiplier;
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

	FMonsterCombatActionInfo() {};
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

	FMonsterNonCombatActionInfo() {};
};

USTRUCT(BlueprintType)
struct FBossWeaknessStats
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<int32> head;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<int32> neck;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<int32> body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<int32> tail;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<int32> wing;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TArray<int32> leg;

	FBossWeaknessStats() {};
	FBossWeaknessStats(const TArray<int32>& head_weakness, const TArray<int32>& neck_weakness,
		const TArray<int32>& body_weakness,	const TArray<int32>& tail_weakness,
		const TArray<int32>& wing_weakness, const TArray<int32>& leg_weakness)
	{
		head = head_weakness;
		neck = neck_weakness;
		body = body_weakness;
		tail = tail_weakness;
		wing = wing_weakness;
		leg  = leg_weakness;
	}
};

USTRUCT(BlueprintType)
struct FBossPartBreakStats
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 damage_head;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 damage_body;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 damage_wing;

	FBossPartBreakStats() {};
	FBossPartBreakStats(const int32 break_damage_head, const int32 break_damage_body, const int32 break_damage_wing)
	{
		this->damage_head = break_damage_head;
		this->damage_body = break_damage_body;
		this->damage_wing = break_damage_wing;
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

	FBossRageStats() {};
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

	FBossEscapeStats() {};
	FBossEscapeStats(const int32 num_allowed_escapes, const TArray<float>& escape_health_ratios)
	{
		this->num_allowed_escapes = num_allowed_escapes;
		this->escape_health_ratios = escape_health_ratios;
	}
};

USTRUCT(BlueprintType)
struct FMonsterSightConfigs
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 radius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 lose_radius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) float fov;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 max_age;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly) int32 auto_success_range;

	FMonsterSightConfigs() {};
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
struct FBossBodyPartInfo
{
	GENERATED_BODY()
	
	//TODO move BossWeaknessStats and BossPartBreakStats here when implementing damage by body part
	//amount of damage needed to stun etc 
};

USTRUCT(BlueprintType)
struct FBossBodyCondition
{
	GENERATED_BODY()
	
	//TODO states of body parts (e.g. broken, damaged, etc)

	void Reset() {};
};

// USTRUCT()
// struct FAIMoveRequestResult
// {
// 	GENERATED_BODY()
// 	
// 	FAIRequestID move_request_id;
// 	EMoveRequestResult result_code;
// };

// End of Monster
//============================================