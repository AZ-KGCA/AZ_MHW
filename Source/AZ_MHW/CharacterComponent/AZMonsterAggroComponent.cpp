// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterAggroComponent.h"
#include "AZ_MHW/Character/Player/AZPlayer_Origin.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameMode/AZGameMode_Server.h"
#include "AZ_MHW/Manager/AZObjectMgr_Server.h"


UAZMonsterAggroComponent::UAZMonsterAggroComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	update_rate_ = 2.0f;
}

void UAZMonsterAggroComponent::Init()
{
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster_Aggro, Error, TEXT("[AZMonsterAggroComponent] Invalid owner actor!"));
	}
	percept_range_ = owner_->percept_radius_;
}

void UAZMonsterAggroComponent::ActivateSystem()
{
	UE_LOG(AZMonster_Aggro, Warning, TEXT("[AZMonsterAggroComponent] Activated"));
	FTimerDelegate timer_delegate;
	timer_delegate.BindUObject(this, &UAZMonsterAggroComponent::UpdateAggro);
	owner_->GetWorld()->GetTimerManager().SetTimer(update_timer_handle_, timer_delegate, update_rate_, true);
}

void UAZMonsterAggroComponent::InactivateSystem()
{
	UE_LOG(AZMonster_Aggro, Warning, TEXT("[AZMonsterAggroComponent] Inactivated"));
	if (owner_->GetWorld()->GetTimerManager().IsTimerActive(update_timer_handle_))
	{
		owner_->GetWorld()->GetTimerManager().ClearTimer(update_timer_handle_);
	}
}

void UAZMonsterAggroComponent::Reset()
{
	target_serial_ = INDEX_NONE;
	target_ref_ = nullptr;
	aggro_info_.Empty();
	InactivateSystem();
}

void UAZMonsterAggroComponent::ForceSetBestTarget(AAZPlayer_Origin* character)
{
	target_serial_ = character->object_serial_;
}

int32 UAZMonsterAggroComponent::GetTargetSerial()
{
	if (aggro_info_.IsEmpty()) return INDEX_NONE;
	SortAggroInfo();
	return target_serial_;
}

AAZPlayer_Origin* UAZMonsterAggroComponent::GetTargetRef() 
{
	return Cast<AAZPlayer_Origin>(target_ref_);
}

FVector UAZMonsterAggroComponent::GetTargetLocation()  
{
	if (!target_ref_.IsValid())
		return FVector::ZeroVector;
	else
		return target_ref_->GetActorLocation();
}

FVector UAZMonsterAggroComponent::GetRandomTargetLocation()
{
	const int rand_idx = FMath::RandRange(0, aggro_info_.Num()-1);
	auto object_mgr = Cast<AAZGameMode_Server>(Cast<UAZGameInstance>(owner_->GetWorld()->GetGameInstance())->GetGameMode())->object_mgr_;
	return object_mgr->GetPlayer(aggro_info_[rand_idx].Key)->GetActorLocation();
}

float UAZMonsterAggroComponent::GetDistance2DToTarget() 
{
	if (!target_ref_.IsValid())
		return 0.0f;
	else
		return FVector::Dist2D(owner_->GetActorLocation(), GetTargetLocation());
}

float UAZMonsterAggroComponent::GetAngle2DToTarget() 
{
	if (!target_ref_.IsValid())
		return 0.0f;
	else
		return owner_->GetRelativeAngleToLocation(GetTargetLocation());
}

void UAZMonsterAggroComponent::SortAggroInfo()
{
	if (aggro_info_.IsEmpty())
	{
		target_serial_ = INDEX_NONE;
		target_ref_ = nullptr;
	}
	// 순서 need check
	aggro_info_.Sort([](TPair<int32, int32> player_A, TPair<int32, int32> player_B) {
		return player_A.Value < player_B.Value;
	});
	target_serial_ = aggro_info_.Top().Key;
	target_ref_ = Cast<AAZGameMode_Server>(Cast<UAZGameInstance>(owner_->GetWorld()->GetGameInstance())->GetGameMode())
		->object_mgr_->GetPlayer(target_serial_);
}

void UAZMonsterAggroComponent::UpdateAggro()
{
	SortAggroInfo();
	UpdateByRange();
}

TPair<int32, int32>* UAZMonsterAggroComponent::FindByKey(int32 player_serial)
{
	for (auto &info : aggro_info_)
	{
		if (info.Key == player_serial) return &info;
	}
	return nullptr;
}

void UAZMonsterAggroComponent::UpdateAggroSpecific(int32 player_serial, int32 aggro_point, FString update_reason)
{
	if (auto info = FindByKey(player_serial))
	{
		info->Value += aggro_point;
		if (info->Value <= 0)
		{
			RemoveTarget(info->Key);
		}
		else
		{
			UE_LOG(AZMonster_Aggro, Log, TEXT("[UAZMonsterAggroComponent][%d] Player %d aggro updated by %d due to [%s]. Current point: %d"),
				owner_->object_serial_, player_serial, aggro_point, *update_reason, info->Value);
		}
	}
	else
	{
		if (aggro_point <= 0) return;
		aggro_info_.Add(MakeTuple(player_serial, aggro_point));
		UE_LOG(AZMonster_Aggro, Warning, TEXT("[UAZMonsterAggroComponent][%d] Player %d added to aggro list due to [%s]. Current point: %d"),
			owner_->object_serial_, player_serial, *update_reason, aggro_point);
	}
}

void UAZMonsterAggroComponent::RemoveTarget(int32 player_serial)
{
	for (int32 i = 0; i < aggro_info_.Num(); i++)
	{
		if (aggro_info_[i].Key == player_serial)
		{
			aggro_info_.RemoveAt(i);
			UE_LOG(AZMonster_Aggro, Log, TEXT("[UAZMonsterAggroComponent][%d] Player %d removed from the aggro list!"),
				owner_->object_serial_, player_serial);
			break;
		}
	}
}

void UAZMonsterAggroComponent::IncreaseByDamage(int32 attacker_serial, int32 damage)
{
	UpdateAggroSpecific(attacker_serial, damage * 0.4, TEXT("Damage"));
}

void UAZMonsterAggroComponent::UpdateByRange()
{
	// Do sphere overlap to find players in range
	TArray<AActor*, FDefaultAllocator> ignore_actors;
	TArray<AActor*> actors_in_range;
	ignore_actors.Add(owner_.Get());
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), owner_->GetActorLocation(), percept_range_,
		owner_->hit_object_types_, AAZPlayer_Origin::StaticClass(), ignore_actors, actors_in_range);

	// Get serial number of actors in range
	// Increase aggro point for players in range
	TArray<int32> serials_in_range;
	for (auto actor : actors_in_range)
	{
		if (auto player = Cast<AAZPlayer_Origin>(actor))
		{
			serials_in_range.Add(player->object_serial_);
			UpdateAggroSpecific(player->object_serial_, 10, "InRange");
		}
	}

	// Decrease aggro point for players not in range
	for (auto info : aggro_info_)
	{
		if (!serials_in_range.Contains(info.Key))
		{
			UpdateAggroSpecific(info.Key, -30, "NotInRange");
		}
	}
}

void UAZMonsterAggroComponent::UpdateByEnterCombat(int32 player_serial)
{
	UpdateAggroSpecific(player_serial, 70, "EnterCombat");
}

void UAZMonsterAggroComponent::IncreaseByPartChange(int32 attacker_serial, EMonsterBodyPartChangeType change_type)
{
	switch (change_type)
	{
		case (EMonsterBodyPartChangeType::Wound) :
		{
			UpdateAggroSpecific(attacker_serial, 30, "PartWound");
			break;
		}
		case (EMonsterBodyPartChangeType::Break) :
		{
			UpdateAggroSpecific(attacker_serial, 50, "PartBreak");
			break;
		}
		case (EMonsterBodyPartChangeType::Sever) :
		{
			UpdateAggroSpecific(attacker_serial, 100, "PartSever");
			break;
		}
	}
}
