// Copyright Team AZ. All Rights Reserved.

#include "AZAnimNotifyState_DoBodyOverlap.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"

UAZAnimNotifyState_DoBodyOverlap::UAZAnimNotifyState_DoBodyOverlap()
{
}

void UAZAnimNotifyState_DoBodyOverlap::NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
                                                 float total_duration, const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyBegin(mesh_comp, animation, total_duration, event_reference);
	if (mesh_comp == nullptr || mesh_comp->GetOwner() == nullptr)
		return;

	if (mesh_comp->GetOwner()->IsA(AAZMonster::StaticClass()))
	{
		AAZMonster* monster = Cast<AAZMonster>(mesh_comp->GetOwner());
		if (monster)
		{
			monster->AnimNotifyState_DoBodyOverlap_Begin();
		}
	}
}

void UAZAnimNotifyState_DoBodyOverlap::NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::NotifyEnd(mesh_comp, animation, event_reference);
	if (mesh_comp == nullptr || mesh_comp->GetOwner() == nullptr)
		return;

	if (mesh_comp->GetOwner()->IsA(AAZMonster::StaticClass()))
	{
		AAZMonster* monster = Cast<AAZMonster>(mesh_comp->GetOwner());
		if (monster)
		{
			monster->AnimNotifyState_DoBodyOverlap_End();
		}
	}
}
