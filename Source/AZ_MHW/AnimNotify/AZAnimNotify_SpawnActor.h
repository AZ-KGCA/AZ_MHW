// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AZAnimNotify_SpawnActor.generated.h"

/**
 * 스테이트로는 궤적보여주고
 * 엔드하면 생성 발사 애니메이션 초입부에 깔기
 */
UCLASS()
class AZ_MHW_API UAZAnimNotify_SpawnActor : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAZAnimNotify_SpawnActor();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	//생성 소켓(활이나 보우건 총구?)

	//생성할 투사체 액터

	//방향 속도
};
