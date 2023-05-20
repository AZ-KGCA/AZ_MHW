// Copyright Team AZ. All Rights Reserved.


#include "AZPlayer_Origin.h"


AAZPlayer_Origin::AAZPlayer_Origin()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AAZPlayer_Origin::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZPlayer_Origin::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZPlayer_Origin::BeginDestroy()
{
	Super::BeginDestroy();
}

float AAZPlayer_Origin::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result,
	AController* event_instigator, const FAttackInfo& attack_info)
{
	return Super::ApplyDamage_Implementation(damaged_actor, hit_result, event_instigator, attack_info);
}

float AAZPlayer_Origin::ProcessDamage(const FHitResult& hit_result, AController* event_instigator,
	const FAttackInfo& attack_info, float applied_damage)
{
	return Super::ProcessDamage(hit_result, event_instigator, attack_info, applied_damage);
}




