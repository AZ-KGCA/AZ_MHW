// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZGameMode.h"
#include "AZGameMode_Server.generated.h"


/**
 * 서버에서 필요한 방식으로 프로그램을 실행합니다.
 * 캐릭터, 컨트롤러 , HUD 등
 */
UCLASS()
class AZ_MHW_API AAZGameMode_Server : public AAZGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAZGameMode_Server();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


};
