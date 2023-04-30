#pragma once
#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "AZAIDefine.generated.h"

namespace AZBlackboardKey
{
	FName const ai_state("AIState");
	FName const action_mode("ActionMode");
	FName const move_state("MoveState");
	FName const target_character("TargetCharacter");
	FName const target_location("TargetLocation");
	FName const wait_time("WaitTime");
	FName const is_in_combat("IsInCombat");
	FName const is_triggered_by_sight("IsTriggeredBySight");
}

UENUM(BlueprintType)
enum class ESearchEnemyType : uint8
{
	None,
	Distance,
	Damage
};