// Copyright Team AZ. All Rights Reserved.


#include "AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_Server.h"
#include "AZ_MHW/Manager/AZObjectMgr_Server.h"


AAZPlayer_Origin::AAZPlayer_Origin()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// 현수오빠 수정전까지 주석처리
// float AAZPlayer_Origin::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result,
// 	AController* event_instigator, const FAttackInfo& attack_info)
// {
// 	return Super::ApplyDamage_Implementation(damaged_actor, hit_result, event_instigator, attack_info);
// }
//
// float AAZPlayer_Origin::ProcessDamage(const FHitResult& hit_result, AController* event_instigator,
// 	const FAttackInfo& attack_info, float applied_damage)
// {
// 	return Super::ProcessDamage(hit_result, event_instigator, attack_info, applied_damage);
// }




