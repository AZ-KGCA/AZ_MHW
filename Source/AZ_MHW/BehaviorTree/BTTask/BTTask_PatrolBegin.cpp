// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/BehaviorTree/BTTask/BTTask_PatrolBegin.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "AZ_MHW/Controller/AZAIController.h"
#include "AZ_MHW/BehaviorTree/AZAIDefine.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include <AZ_MHW/Character/Monster/AZMonster.h>

UBTTask_PatrolBegin::UBTTask_PatrolBegin()
{
	NodeName = "Begin Patrol";
}

EBTNodeResult::Type UBTTask_PatrolBegin::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto ai_controller = Cast<AAZAIController>(owner_comp.GetAIOwner());
	if (!ai_controller)
		return EBTNodeResult::Failed;

	auto ai_pawn = ai_controller->GetPawn();
	if (!ai_pawn)
		return EBTNodeResult::Failed;

	double patrol_range = ai_controller->patrol_range_;
	if (!patrol_range)
		return EBTNodeResult::Failed;

	UWorld* world = owner_comp.GetWorld();
	if (!world)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* nav_system = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!nav_system)
		return EBTNodeResult::Failed;

	//TODO: 주변에 플레이어 없으면 patrol 하지 않음
	//TODO: 스폰지점보다 멀면 스폰지점으로 이동

	FVector curr_position = ai_pawn->GetActorLocation();
	FVector dest_position = curr_position;
	FNavLocation found_position;

	if (nav_system->GetRandomPointInNavigableRadius(curr_position, patrol_range, found_position))
	{
		dest_position = found_position.Location;
	}

	owner_comp.GetBlackboardComponent()->SetValueAsVector(AZBlackboardKey::target_location, dest_position);
	owner_comp.GetBlackboardComponent()->SetValueAsFloat(AZBlackboardKey::wait_time, ai_controller->patrol_delay_);

	return EBTNodeResult::Succeeded;
}
