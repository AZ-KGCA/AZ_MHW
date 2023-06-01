// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AZAnimNotifyState_Roar.generated.h"

class AAZPlayer_Origin;
/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZAnimNotifyState_Roar : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration, const FAnimNotifyEventReference& event_reference) override;
	virtual void NotifyTick(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, float total_duration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;

private:
	UPROPERTY() TArray<AAZPlayer_Origin*> target_players_;
};
