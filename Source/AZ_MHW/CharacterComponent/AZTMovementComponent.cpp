// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/CharacterComponent/AZTMovementComponent.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "AZ_MHW/AZ_MHW.h"
#include <AZ_MHW/Controller/AZAIController.h>

// Sets default values for this component's properties
UAZTMovementComponent::UAZTMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
	
}

void UAZTMovementComponent::InitializeComponent()
{
	Super::InitializeComponent();

	if (GetOwner())
	{
		owner_ = Cast<AAZCharacter>(GetOwner());
		move_state_ = EMoveState::Stop;
	}
}

// Called when the game starts
void UAZTMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAZTMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


bool UAZTMovementComponent::IsMovable()
{
	//TODO
	return true;
}




float UAZTMovementComponent::GetMoveSpeed()
{
	// TODO
	//switch (move_type_)
	//{
	//case EMoveType::Stop:
	//case EMoveType::Walk:
	//case EMoveType::Run:
	//	return owner_->walk_speed_;
	//}
	//return owner_->walk_speed_;
	return 0.0f;
}