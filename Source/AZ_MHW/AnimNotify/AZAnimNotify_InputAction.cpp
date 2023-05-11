// Copyright Team AZ. All Rights Reserved.


#include "AZAnimNotify_InputAction.h"

UAZAnimNotify_InputAction::UAZAnimNotify_InputAction()
{
}

void UAZAnimNotify_InputAction::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	
}
