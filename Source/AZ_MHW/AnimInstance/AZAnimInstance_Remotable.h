// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZAnimInstance_Player.h"
#include "AZAnimInstance_Remotable.generated.h"

/**
 * 이 클래스는 서버한테 다른 플레이어의 액션 실행결과값만 받아서 실행하면 됨.
 * 주변환경을 감지하거나, 상태값을 갱신하고 애님인스턴스가 해석할 필요가 없다.
 * 사실상 그냥 Playable이 될 수 없는 Player와 같다.
 */
UCLASS()
class AZ_MHW_API UAZAnimInstance_Remotable : public UAZAnimInstance_Player
{
	GENERATED_BODY()
	
public:
	UAZAnimInstance_Remotable();

#pragma region //Inherited function
protected:
	/** */
	virtual void NativeInitializeAnimation() override;
	/** */
	virtual void NativeUpdateAnimation(float delta_seconds) override;
#pragma endregion
};
