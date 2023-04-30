// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/CharacterComponent/AZAggroComponent.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/AZ_MHW.h"


// Sets default values for this component's properties
UAZAggroComponent::UAZAggroComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}

void UAZAggroComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner())
	{
		owner_ = Cast<AAZMonster>(GetOwner());
	}
}

// Called when the game starts
void UAZAggroComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

/*
@TODO Find best target
Currently just returns the first player character idx
*/
int32 UAZAggroComponent::GetBestTargetSerial()
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

AAZCharacter* UAZAggroComponent::GetRefToTarget() const
{
	return target_.Get();
}

FVector UAZAggroComponent::GetTargetLocation() const 
{
	return target_->GetActorLocation();
}

float UAZAggroComponent::GetDistance2DToTarget() const
{
	return FVector::Dist2D(owner_->GetActorLocation(), GetTargetLocation());
}