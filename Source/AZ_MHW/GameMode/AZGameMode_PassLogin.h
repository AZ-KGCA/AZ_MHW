// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZGameMode.h"
#include "AZGameMode_PassLogin.generated.h"

/**
 * 인게임 개발자용 로그인 모드
 * 
 */
UCLASS()
class AZ_MHW_API AAZGameMode_PassLogin : public AAZGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAZGameMode_PassLogin();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

 };
	
