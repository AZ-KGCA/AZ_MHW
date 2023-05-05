// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AZAnimNotify_ActionStart.h"

UAZAnimNotify_ActionStart::UAZAnimNotify_ActionStart()
{
}

void UAZAnimNotify_ActionStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
}
