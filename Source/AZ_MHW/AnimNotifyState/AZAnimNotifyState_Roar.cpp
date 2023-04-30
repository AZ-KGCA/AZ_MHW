// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/AnimNotifyState/AZAnimNotifyState_Roar.h"
#include "AZ_MHW/Util/AZUtility.h"

void UAZAnimNotifyState_Roar::NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyBegin(mesh_comp, animation, total_duration, event_reference);
	UE_LOG(AZMonster, Warning, TEXT("ROAR begin"));
}

void UAZAnimNotifyState_Roar::NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyEnd(mesh_comp, animation, event_reference);
	UE_LOG(AZMonster, Warning, TEXT("ROAR end"));
}
