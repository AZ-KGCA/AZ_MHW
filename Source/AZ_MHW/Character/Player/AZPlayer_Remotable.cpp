// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayer_Remotable.h"

AAZPlayer_Remotable::AAZPlayer_Remotable()
{
}

float AAZPlayer_Remotable::ApplyDamage_Implementation(AActor* damaged_actor,
	const FHitResult& hit_result, AController* event_instigator, const FAttackInfo& attack_info)
{
	// Do nothing in remote player
	return 0.f;
}
