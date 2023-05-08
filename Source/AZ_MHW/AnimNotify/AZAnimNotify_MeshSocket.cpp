// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/AZAnimNotify_MeshSocket.h"

UAZAnimNotify_MeshSocket::UAZAnimNotify_MeshSocket()
{
}

void UAZAnimNotify_MeshSocket::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
}
