// Copyright Team AZ. All Rights Reserved.


#include "AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_Server.h"
#include "AZ_MHW/Manager/AZObjectMgr_Server.h"


AAZPlayer_Origin::AAZPlayer_Origin()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAZPlayer_Origin::BeginPlay()
{
	Super::BeginPlay();
	Cast<AAZGameMode_Server>(game_instance_->GetGameMode())->object_mgr_->AddObject(this);
}

void AAZPlayer_Origin::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZPlayer_Origin::BeginDestroy()
{
	Super::BeginDestroy();
}

float AAZPlayer_Origin::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info)
{
	return Super::ApplyDamage_Implementation(damaged_actor, hit_result, attack_info);
}

float AAZPlayer_Origin::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	return Super::ProcessDamage(damage_instigator, hit_result, attack_info);
}








