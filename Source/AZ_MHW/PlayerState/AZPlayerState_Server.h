// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayerState_Client.h"
#include "AZPlayerState_Server.generated.h"

/**
 * 서버 설정용
 */
UCLASS()
class AZ_MHW_API AAZPlayerState_Server : public AAZPlayerState_Client
{
	GENERATED_BODY()

public:
	AAZPlayerState_Server();

protected:
	virtual void BeginPlay() override;

	virtual void BeginDestroy() override;
};
