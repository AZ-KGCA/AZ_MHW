// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AZAnimInstance_Player.h"
#include "PlayerState/AZPlayerState.h"

UAZAnimInstance_Player::UAZAnimInstance_Player()
{

}

void UAZAnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if(const APawn* OwnerPawn = TryGetPawnOwner())
	{
		if(AAZPlayerState* AZPlayerState =  Cast<AAZPlayerState>(OwnerPawn->GetPlayerState()))
		{
			AZPlayerStateCache = AZPlayerState;	
		}
	}
}

void UAZAnimInstance_Player::AnimNotify_OnActionStart()
{
	OnActionStart.Broadcast();
}

void UAZAnimInstance_Player::AnimNotify_OnEnableInput()
{
	OnEnableInput.Broadcast();
}

void UAZAnimInstance_Player::AnimNotify_OnEnableAction()
{
	OnEnableAction.Broadcast();
}

void UAZAnimInstance_Player::AnimNotify_OnActionEnd()
{
	OnActionEnd.Broadcast();
}


void UAZAnimInstance_Player::AnimNotify_OnAttackHitCheck()
{
	OnAttackHitCheck.Broadcast();
}

void UAZAnimInstance_Player::AnimNotify_OnCastingCheck()
{
	
}

void UAZAnimInstance_Player::AnimNotify_OnDisableCheck()
{
	
}




FVector UAZAnimInstance_Player::GetVelocity(bool Forward, bool Back, bool Left, bool Right) const
{
	int MovementX =(static_cast<int>(Back)-static_cast<int>(Forward));
	
	int MovementY = (static_cast<int>(Left)-static_cast<int>(Right));
	
	FVector Movement(MovementX,MovementY,0);
	Movement.Normalize();
	return Movement;
}


// void UAZAnimInstance_Player::PlayMontage(FName MontageName)
// {
// 	if(const auto& Montage = *MontageMap.Find(MontageName))
// 	{
// 		//Montage_Play(Montage,1.0f);
// 		//Montage_JumpToSection()
// 	}
// }