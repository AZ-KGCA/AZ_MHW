// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayerController.h"
#include "AZPlayerController_Server.generated.h"



UCLASS()
class AZ_MHW_API AAZPlayerController_Server : public AAZPlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAZPlayerController_Server();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	virtual void SetupInputComponent() override;

	virtual void OnPossess(APawn* pawn) override;

public:
	UPROPERTY()
	TMap<int32, class AAZPlayer_Playable*> logined_player_list_;

	//UFUNCTION() void CreateClonePlayer(int32 guid, AAZPlayerState player_state);
	//서버용 로직과 데이터 받아서 넘겨주기
};
