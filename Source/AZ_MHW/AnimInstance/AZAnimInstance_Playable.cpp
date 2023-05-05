// Fill out your copyright notice in the Description page of Project Settings.


#include "AZAnimInstance_Playable.h"

#include "Character/Player/AZPlayer.h"
#include "PlayerState/AZPlayerState.h"

UAZAnimInstance_Playable::UAZAnimInstance_Playable()
{
	
}

void UAZAnimInstance_Playable::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
}

void UAZAnimInstance_Playable::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if(AAZPlayer* AZPlayer = Cast<AAZPlayer>(TryGetPawnOwner()))
	{
		AZPlayerCache = AZPlayer;
		
		if(AAZPlayerState* AZPlayerState =  Cast<AAZPlayerState>(AZPlayer->GetPlayerState()))
		{
			AZPlayerStateCache = AZPlayerState;	
		}
		
	}
	
		
	//OwnerPawn->GetRootComponent()->GetComponentRotation();//캐릭터 방향
	//(컨트롤러방향) 플레이어 방향 가져오기= InputRotation() 입력 방향 가져오기
	//OwnerPawn->GetWorldPosition();

	//상태변화 체크
	//입력변화 체크
	//액션조건 체크
	//액션 선택
	//액션 실행화

	//현재 액션을 실행하고 실행한다.
	//나중 액션을 실행하고 실행한다.
	//
}

int32 UAZAnimInstance_Playable::GetInputActionBitMask() const
{
	int32 Result = 0;
	if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	if(AZPlayerStateCache->ActionState.bMoveBack) Result|=(1<<1);
	if(AZPlayerStateCache->ActionState.bMoveLeft) Result|=(1<<2);
	if(AZPlayerStateCache->ActionState.bMoveRight) Result|=(1<<3);
	
	if(AZPlayerStateCache->ActionState.bNormalAttack) Result|=(1<<4);
	if(AZPlayerStateCache->ActionState.bSpecialAttack) Result|=(1<<5);
	if(AZPlayerStateCache->ActionState.bUniqueWeaponAction) Result|=(1<<6);
	if(AZPlayerStateCache->ActionState.bEvade) Result|=(1<<7);
	
	if(AZPlayerStateCache->ActionState.bDashHold) Result|=(1<<8);
	if(AZPlayerStateCache->ActionState.bUseItem) Result|=(1<<9);
	
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	// if(AZPlayerStateCache->ActionState.bMoveForward) Result|=(1<<0);
	return Result;
}
