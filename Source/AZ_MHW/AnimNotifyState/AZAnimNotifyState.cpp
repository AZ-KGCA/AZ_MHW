// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotifyState/AZAnimNotifyState.h"

UAZAnimNotifyState::UAZAnimNotifyState()
{
	
}

void UAZAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration)
{
	//Super::NotifyBegin(MeshComp, Animation, TotalDuration); //빈함수
}

void UAZAnimNotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration)
{
	//Super::NotifyTick(MeshComp, Animation, TotalDuration); //빈함수
}

void UAZAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	//Super::NotifyEnd(MeshComp, Animation); //빈함수
}
