// Copyright Team AZ. All Rights Reserved.

#include "AZMonsterAggroComponent.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Util/AZUtility.h"


UAZMonsterAggroComponent::UAZMonsterAggroComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UAZMonsterAggroComponent::InitializeComponent()
{
	Super::InitializeComponent();

	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterAggroComponent] Invalid owner actor!"));
	}
}

void UAZMonsterAggroComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAZMonsterAggroComponent::SetBestTarget(AAZCharacter* character)
{
	best_target_ = character;
}

/*
TODO Find best target
Currently just returns the first player character idx
*/
int32 UAZMonsterAggroComponent::GetBestTargetSerial()
{
	if (GetWorld()->GetNumPlayerControllers() == 0)
	{
		return INDEX_NONE;
	}
	else
	{
		return 0;
	}

	//if (aggro_info_.IsEmpty()) return INDEX_NONE;
}

AAZCharacter* UAZMonsterAggroComponent::GetTargetRef() const
{
	return best_target_.Get();
}

FVector UAZMonsterAggroComponent::GetTargetLocation() const 
{
	if (!best_target_.IsValid())
		return FVector::ZeroVector;
	else
		return best_target_->GetActorLocation();
}

float UAZMonsterAggroComponent::GetDistance2DToTarget() const
{
	if (!best_target_.IsValid())
		return 0.0f;
	else
		return FVector::Dist2D(owner_->GetActorLocation(), GetTargetLocation());
}