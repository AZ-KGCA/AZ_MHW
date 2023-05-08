// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstance/AZAnimInstance_Player.h"
#include "GameInstance/AZGameInstance.h"
#include "Manager/AZPlayerAssetMgr.h"

UAZAnimInstance_Player::UAZAnimInstance_Player()
{
	//CurrentAnimMontage = LoadObject<UAnimMontage>(nullptr,TEXT("Base"),"/Game/Content/Character/Animation/",)
	CurrentAnimPlayRate = 1.5f;
}

void UAZAnimInstance_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
}

void UAZAnimInstance_Player::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//TryGetPawnOwner()->GetPlayerState()
}

void UAZAnimInstance_Player::SetMontage(FName MontageName)
{
	bIsMontage = true;
	if(const auto& MontageAsset = AZGameInstance->playerAsset_mgr->GetMontage(MontageName))
	{
		CurrentAnimMontage = MontageAsset;
	}
	//Base
	//Crouch
	//Hit
	//Guard
	//WP00
	//WP11

	//Grapple
	//Swing
	//Climb
	//Carry

	//Swim
	//Fishing
}

void UAZAnimInstance_Player::SetNextMontage(FName MontageName)
{
	CurrentMontageName = MontageName;
}

//강제로 섹션변경해라
void UAZAnimInstance_Player::SetSection(FName SectionName)
{
	Montage_JumpToSectionsEnd(CurrentSectionName, CurrentAnimMontage);
	CurrentSectionName = SectionName;
	Montage_JumpToSection(CurrentSectionName, CurrentAnimMontage);
}
//현재 섹션끝나면 자연스럽게 옮겨라 노티파이로(연계기)
void UAZAnimInstance_Player::SetNextSection(FName SectionName)
{
	NextSectionName = SectionName;
}

void UAZAnimInstance_Player::SetBlendSpace(FName BlendSpaceName)
{
	bIsMontage = false;
	bIsBlend = true;
	//WASDLockRotate(Guard, Charge?)
}

void UAZAnimInstance_Player::SetSequence(FName SequenceName)
{
	bIsMontage = false;
	bIsBlend = false;
	//SequenceName
	//Gesture
}

void UAZAnimInstance_Player::SetPlayRate(float PlayRate)
{
	if(Montage_IsPlaying(CurrentAnimMontage))
	{
		Montage_SetPlayRate(CurrentAnimMontage, PlayRate);
	}
	CurrentAnimPlayRate = PlayRate;
}
