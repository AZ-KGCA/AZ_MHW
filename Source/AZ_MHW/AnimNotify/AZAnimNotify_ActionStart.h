// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <Animation/AnimNotifies/AnimNotify.h>
#include "AZAnimNotify_ActionStart.generated.h"

/**
 * 몬타주 섹션의 시작을 알림.
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_ActionStart : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAZAnimNotify_ActionStart();

	virtual void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;


};
