// Copyright Team AZ. All Rights Reserved.


#include "AZAnimNotify_SpawnActor.h"

UAZAnimNotify_SpawnActor::UAZAnimNotify_SpawnActor()
{
}

void UAZAnimNotify_SpawnActor::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
}
