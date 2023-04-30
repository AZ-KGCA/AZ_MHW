#include "AZ_MHW/BehaviorTree//BTTask/BTTask_SelectRandomAction.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include <BehaviorTree/Blackboard/BlackboardKeyAllTypes.h>

#include "AIController.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"


UBTTask_SelectRandomAction::UBTTask_SelectRandomAction(const FObjectInitializer& object_initializer)
{
	NodeName = "Select Action";
	key_is_triggered_by_sight.AddBoolFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAction, key_is_triggered_by_sight));
	key_action_mode.AddEnumFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAction, key_action_mode), StaticEnum<EMonsterActionMode>());
	key_target_character.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(UBTTask_SelectRandomAction, key_target_character), AAZCharacter::StaticClass());
}

void UBTTask_SelectRandomAction::InitializeFromAsset(UBehaviorTree& asset)
{
	Super::InitializeFromAsset(asset);

	UBlackboardData* bb_asset = GetBlackboardAsset();
	if (ensure(bb_asset))
	{
		key_is_triggered_by_sight.ResolveSelectedKey(*bb_asset);
		key_action_mode.ResolveSelectedKey(*bb_asset);
		key_target_character.ResolveSelectedKey(*bb_asset);
	}
}

EBTNodeResult::Type UBTTask_SelectRandomAction::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	AAZMonster* owner = Cast<AAZMonster>(owner_comp.GetAIOwner()->GetPawn());
	UBlackboardComponent* bb_comp = owner_comp.GetBlackboardComponent();
	
	EMonsterActionMode action_mode = static_cast<EMonsterActionMode>(owner_comp.GetBlackboardComponent()->GetValueAsEnum(key_action_mode.SelectedKeyName));
	switch (action_mode)
	{
	case EMonsterActionMode::Normal:
		{
			SelectNonCombatAction(owner, bb_comp);
		}
	case EMonsterActionMode::Transition:
		{
			SelectTransitionAction(owner, bb_comp);
		}
	case EMonsterActionMode::Combat:
		{
			SelectCombatAction(owner, bb_comp);
		}
	default:
		return EBTNodeResult::Failed;
	}
}

EBTNodeResult::Type UBTTask_SelectRandomAction::SelectNonCombatAction(AAZMonster* owner, UBlackboardComponent* blackboard)
{
	auto available_actions_map = owner->noncombat_action_map_.FilterByPredicate([&owner, this](const auto& action_map)
	{
		const EMonsterActionConditionType conditions = action_map.Value.conditions;
		if ((conditions & EMonsterActionConditionType::Flying) == EMonsterActionConditionType::Flying)
		{
			// Unacceptable if the owner should be flying but is not
			if (!owner->is_flying_) return false;
		}
		if ((conditions & EMonsterActionConditionType::InRange) == EMonsterActionConditionType::InRange)
		{
			// Unacceptable if a player should be in the perceivable distance but is not
			if (!is_player_in_range) return false;
		}
		if ((conditions & EMonsterActionConditionType::Health) == EMonsterActionConditionType::Health)
		{
			// Unacceptable if the owner health is not within the range
			if (owner->state_info_.hp_ratio < action_map.Value.condition_min_health_ratio) return false;
			if (owner->state_info_.hp_ratio > action_map.Value.condition_max_health_ratio) return false;
		}
		// acceptable if all conditions are met
		return true;
	});

	// If there is no available action, return fail
	if (available_actions_map.IsEmpty())
		return EBTNodeResult::Failed;
	
	// Select random action from available actions
	const int idx = FMath::RandRange(0, available_actions_map.Num());
	owner->SetActionState(available_actions_map[idx].action_id);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_SelectRandomAction::SelectTransitionAction(AAZMonster* owner, UBlackboardComponent* blackboard)
{
	auto available_actions_map = owner->combat_action_map_.FilterByPredicate([&owner, &blackboard, this](const auto& action_map)
	{
		// Filter out non-transition actions
		const EMonsterActionTriggerType triggers = action_map.Value.triggers;
		if ((triggers & EMonsterActionTriggerType::Transition) != EMonsterActionTriggerType::Transition)
		{
			return false;
		}
		// If the action is only triggered by sight, check if the trigger succeeded
		if (((triggers & EMonsterActionTriggerType::Damage) != EMonsterActionTriggerType::Damage)
			&& ((triggers & EMonsterActionTriggerType::Sight) == EMonsterActionTriggerType::Sight))
		{
			if (!blackboard->GetValueAsBool(key_is_triggered_by_sight.SelectedKeyName))
			{
				return false;
			}
		}
		// Filter out actions that fail the conditions
		// Transition action condition checks omit range and health conditions
		const EMonsterActionConditionType conditions = action_map.Value.conditions;
		if ((conditions & EMonsterActionConditionType::Flying) == EMonsterActionConditionType::Flying)
		{
			// Unacceptable if the owner should be flying but is not
			if (!owner->is_flying_) return false;
		}
		// acceptable if all conditions are met
		return true;
	});

	// If there is no available action, return fail
	if (available_actions_map.IsEmpty())
		return EBTNodeResult::Failed;
	
	// Select random action from available actions
	const int idx = FMath::RandRange(0, available_actions_map.Num());
	owner->SetActionState(available_actions_map[idx].action_id);
	return EBTNodeResult::Succeeded;
}

EBTNodeResult::Type UBTTask_SelectRandomAction::SelectCombatAction(AAZMonster* owner, UBlackboardComponent* blackboard)
{
	auto available_actions_map = owner->combat_action_map_.FilterByPredicate([&owner, &blackboard, this](const auto& action_map)
	{
		//TODO currently no trigger is implemented for combat actions
		const EMonsterActionTriggerType triggers = action_map.Value.triggers;
		if (triggers != EMonsterActionTriggerType::None)
		{
			return false;
		}
		// Filter out actions that fail the conditions
		const EMonsterActionConditionType conditions = action_map.Value.conditions;
		if (conditions == EMonsterActionConditionType::None)
		{
			// Acceptable if there is no conditions to check for
			return true;
		}
		if ((conditions & EMonsterActionConditionType::Flying) == EMonsterActionConditionType::Flying)
		{
			// Unacceptable if the owner should be flying but is not
			if (!owner->is_flying_) return false;
		}
		if ((conditions & EMonsterActionConditionType::InRange) == EMonsterActionConditionType::InRange)
		{
			// Unacceptable if the target player is not in range
			FVector target_location = Cast<AActor>(blackboard->GetValueAsObject(key_target_character.SelectedKeyName))->GetActorLocation();
			float distance_to_target = owner->GetDistance2DToLocation(target_location);
			if (distance_to_target < action_map.Value.condition_min_distance_from_target) return false;
			if (distance_to_target > action_map.Value.condition_max_distance_from_target) return false;
		}
		if ((conditions & EMonsterActionConditionType::Health) == EMonsterActionConditionType::Health)
		{
			// Unacceptable if health condition is not met
			//TODO currently not implemented
		}
		//TODO cooltime
		// acceptable if all conditions are met
		return true;
	});

	// If there is no available action, return fail
	if (available_actions_map.IsEmpty())
		return EBTNodeResult::Failed;
	
	// Select random action from available actions
	const int idx = FMath::RandRange(0, available_actions_map.Num());
	owner->SetActionState(available_actions_map[idx].action_id);
	return EBTNodeResult::Succeeded;
}
