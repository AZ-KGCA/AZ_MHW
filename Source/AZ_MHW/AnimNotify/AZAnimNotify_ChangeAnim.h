// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_ChangeAnim.generated.h"

/**
 * 해당 노티파이가 실행되면 애님클래스의 설정을 변경하는 알림클래스
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_ChangeAnim : public UAnimNotify
{
	GENERATED_BODY()
	
public:
	UAZAnimNotify_ChangeAnim();

	virtual  void Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation, const FAnimNotifyEventReference& event_reference) override;
public:
	UPROPERTY(EditAnywhere)
	bool is_rotate_;
};
