// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZAnimNotifyState_DoBodyOverlap.generated.h"

UCLASS()
class AZ_MHW_API UAZAnimNotifyState_DoBodyOverlap : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	UAZAnimNotifyState_DoBodyOverlap();

	virtual void NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration, const FAnimNotifyEventReference& event_reference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
};
