// Copyright Team AZ. All Rights Reserved.


#include "AZAnimNotify_ChangeAnim.h"

UAZAnimNotify_ChangeAnim::UAZAnimNotify_ChangeAnim()
{
}

void UAZAnimNotify_ChangeAnim::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);
}
