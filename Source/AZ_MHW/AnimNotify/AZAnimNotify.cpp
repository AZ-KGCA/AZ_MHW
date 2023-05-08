// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AZAnimNotify.h"

UAZAnimNotify::UAZAnimNotify()
{
}

/**
 * MeshComp = 애니메이션 실행자
 * Animation = 현재 실행중인 애니메이션
 */
void UAZAnimNotify::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);//빈 함수

	// FHitResult HitResult;
	// FCollisionQueryParams Params(NAME_None,false,this);
	// bool bResult = GetWorld()->SweepSingleByChannel(
	// 	HitResult,
	// 	GetActorLocation(),
	// 	GetActorLoaction()+GetActorForwardVector()+AttackRange,
	// 	FQuat::Identity,
	// 	ECollisionChannel::ECC_EngineTraceChannel1,//
	// 	FCollisionShape::MakeSphere(AttakRadius),//
	// 	Params);

	
#if ENABLE_DRAW_DEBUG
	// FVector TraceVec = 방향 * 공격범위;
	// FVector Center = GetActorLocation+트레이스방향*0.5;
	// float HalfHeight = AttackRange * 0.5f + AttackRadius;
	// FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
	// FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	// float DebugLifeTime = 5.0f;
	// DrawDebugCapsule(GetWorld(),
	// 	ENodeZone::Center,
	// 	HalfHeight,
	// 	AttackRadius,
	// 	CapsuleRot,
	// 	DrawColor,
	// 	false,
	// 	DebugLifeTime);
#endif

	// if(bResult)
	// {
	// 	if(HitResult.GetActor()->IsValidLowLevel())
	// 	{
	// 		PRINT_LOG("데미지");	
	// 	}
	// }
}
