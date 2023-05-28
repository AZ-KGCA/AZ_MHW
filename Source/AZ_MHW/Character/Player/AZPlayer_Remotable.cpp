// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayer_Remotable.h"

AAZPlayer_Remotable::AAZPlayer_Remotable()
{
}

void AAZPlayer_Remotable::BeginPlay()
{
	Super::BeginPlay();
}

void AAZPlayer_Remotable::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZPlayer_Remotable::BeginDestroy()
{
	Super::BeginDestroy();
}

float AAZPlayer_Remotable::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result,
	FAttackInfo attack_info)
{
	return 0.f;
}

float AAZPlayer_Remotable::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result,
	FAttackInfo attack_info)
{
	return 0.f;
}


