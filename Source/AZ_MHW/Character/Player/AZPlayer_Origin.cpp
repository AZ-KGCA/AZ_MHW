// Copyright Team AZ. All Rights Reserved.


#include "AZPlayer_Origin.h"
#include "GameInstance/AZGameInstance.h"
#include "GameMode/AZGameMode_Server.h"
#include "AZ_MHW/Manager/AZObjectMgr_Server.h"
#include "Components/CapsuleComponent.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/Manager/AZInventoryManager.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "AZ_MHW/Item/AZItemData.h"
#include <Components/CapsuleComponent.h>

AAZPlayer_Origin::AAZPlayer_Origin()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AAZPlayer_Origin::BeginPlay()
{
	Super::BeginPlay();
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	Cast<AAZGameMode_Server>(game_instance_->GetGameMode())->object_mgr_->AddObject(this);
	// Damage Interface
	OnTakeDamage.AddDynamic(this, &AAZPlayer_Origin::PostProcessDamage);
}

void AAZPlayer_Origin::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);
}

void AAZPlayer_Origin::BeginDestroy()
{
	// Damage Interface
	OnTakeDamage.RemoveDynamic(this, &AAZPlayer_Origin::PostProcessDamage);
	Super::BeginDestroy();
}




float AAZPlayer_Origin::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info)
{
	return Super::ApplyDamage_Implementation(damaged_actor, hit_result, attack_info);
	
}

float AAZPlayer_Origin::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	return Super::ProcessDamage(damage_instigator, hit_result, attack_info);
	//float angle
	//characterstate - bithit =true
	//broadcast, send
}

void AAZPlayer_Origin::PostProcessDamage(AActor* damage_instigator, const FHitResult hit_result,
	FAttackInfo attack_info)
{
	//최종 데미지처리후에 체력이 0이하라면 죽음처리..<<애님노티파이에서도 확인 함
	
	// //Origin과 Playable
	// AAZMonster* instigator_monster = Cast<AAZMonster>(damage_instigator);
	// if (!instigator_monster)
	// {
	// 	UE_LOG(AZMonster, Warning, TEXT("[AAZPlayer] Damage dealt by non-AZMonster actor %s"), *instigator_monster->GetName());
	// 	return;
	// }
	// 	
	// //함수화하기
	// const FVector forward_direction = GetActorForwardVector();
	// const FVector from_hit_direction = (instigator_monster->GetActorLocation());
	// const FVector to_hit_direction = (from_hit_direction - GetActorLocation()).GetSafeNormal();
	//
	// const double cos_theta = FVector::DotProduct(forward_direction,to_hit_direction);
	// double theta = FMath::Acos(cos_theta);
	// theta = FMath::RadiansToDegrees(theta);
	//
	// const FVector cross_vector = FVector::CrossProduct(forward_direction,to_hit_direction);
	// if(cross_vector.Z<0)
	// {
	// 	theta *=-1.f;
	// }
	// //피해 타입(약(살짝경직),중(날라가지만 자세),강(날라가서구르기))에 따라 강제회전타입(앞뒤 넉백 에어본)도 넣기
	// //Default Back
	// if(theta >=-45.f && theta < 45.f)
	// {
	// 	//Front
	// 	UE_LOG(AZ_DAMAGE,Error,TEXT("Front"));
	// 	//Cast<UAZAinmInstance_Player>(GetMesh()->GetAnimInstance())->SetMontageName(TEXT(Hit Front));
	// }
	// else if(theta >= -135.f && theta <-45.f)
	// {
	// 	//Left
	// 	UE_LOG(AZ_DAMAGE,Error,TEXT("Left"));
	// }
	// else if(theta >=45.f && theta < 135.f)
	// {
	// 	//Right
	// 	UE_LOG(AZ_DAMAGE,Error,TEXT("Right"));
	// }
	// 	
	// 	
	// if(const auto player_state = GetPlayerState<AAZPlayerState_Client>())
	// {
	// 	//TODO 공격타입별로 데미지 계산, attack_info.damage_array에 TTuple<EDamageType, int32> 꼴로 있습니다
	// 	player_state->character_state_.current_health_point -= attack_info.GetDamageTotal();
	// 	if(player_state->character_state_.current_health_point <= 0)
	// 	{
	// 		player_state->character_state_.current_health_point = 0;
	// 	}
	//
	// 	if (GEngine)
	// 	{
	// 		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player Health: %d"),
	// 			player_state->character_state_.current_health_point));
	// 	}
	// }
	//
	//
	//
}



void AAZPlayer_Origin::AnimNotify_OnUseItem()
{
	//서버처리
	//on_use_item_.Broadcast();
	//const auto& item_index;// ui manager한테서 가져오기
	const auto& buff = game_instance_->game_cache_info_->GetCurrentCharacterItem()->UsePotion(0);

	switch (buff.target)
	{
	case EItemTarget::health:
		if(auto player_state = GetPlayerState<AAZPlayerState_Client>())
		{
			int32 result_health_point = 0;
			switch (buff.calc)
			{
			case ECalculation::plus:
				result_health_point = player_state->character_state_.current_health_point + (int)buff.effect;
				if(player_state->character_state_.max_health_point < result_health_point)
				{
					player_state->character_state_.current_health_point = player_state->character_state_.max_health_point;
				}
				else
				{
					player_state->character_state_.current_health_point = result_health_point;
				}
				UE_LOG(AZ_TEST,Warning,TEXT("%d"),player_state->character_state_.current_health_point);
				break;
			case ECalculation::multi:
				break;
			}
		}
		break;
	case EItemTarget::damage:
		if(auto player_state = GetPlayerState<AAZPlayerState_Client>())
		{
			if(auto az_player_state = Cast<AAZPlayerState_Client>(player_state))
			{
				switch (buff.calc)
				{
				case ECalculation::plus:
					UE_LOG(AZ_TEST,Warning,TEXT("공격력 더해짐"));
					break;
				case ECalculation::multi:
					UE_LOG(AZ_TEST,Warning,TEXT("공격력 곱해서 올라감"));
					break;
				}
			}
		}
		break;
	}
	//사용한 아이템을 id로 넘겨줘야한다면 oneparam쓰기
	//또는 플레이어스테이트를 캐싱해서 사용한 아이템 체크하기
}

void AAZPlayer_Origin::AnimNotify_OnGetItem()
{
}






