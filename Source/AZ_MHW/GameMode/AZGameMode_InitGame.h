// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZGameMode.h"
#include "AZGameMode_InitGame.generated.h"

/**
 * 게임실행시 명령줄 인수로 서버와 클라이언트의 게임모드를 분기합니다.
 */
UCLASS()
class AZ_MHW_API AAZGameMode_InitGame : public AAZGameMode
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAZGameMode_InitGame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void InitGame(const FString& map_name, const FString& options, FString& error_message) override;

	UFUNCTION(BlueprintCallable)
	void PlayServerMode();
	UFUNCTION(BlueprintCallable)
	void PlayClientMode();
};
