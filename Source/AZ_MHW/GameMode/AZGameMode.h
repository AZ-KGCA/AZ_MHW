// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AZ_MHW/ClientMsg/AZMsgObject.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/GameStateMachine/AZStateMachine.h"
#include "AZGameMode.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZGameMode : public AGameModeBase, public IAZMsgObject
{
	GENERATED_BODY()

public:
	AAZGameMode();
	~AAZGameMode();

	virtual void InitGame(const FString& map_name, const FString& , FString& ErrorMessage) override;
	virtual void InitGameState() override;
	virtual void StartPlay() final;
	virtual void GetSeamlessTravelActorList(bool bToTransition, TArray<AActor*>& ActorList) override;
	virtual void Tick(float delta_seconds) override;
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason) override;

	virtual void OnGameMsg(struct FAZGameMsg* new_msg) override;
	virtual void BeginPlay() override;

public:
	class UAZStateMachine* GetStateMachine() const;
	void SetGameState(EGameState state_id);
	void SetPrevGameState();
	class UAZGameState* GetCurGameState() const;
	UAZGameState* GetGameState(EGameState state_id) const;

	EGameState GetCurGameStateID() const;

public:
	virtual void PreStartPlay() {}
	virtual void PostStartPlay() {}
	virtual EGameModeType GetGameModeType() { return EGameModeType::None; }

protected:
	EGameModeFlag game_mode_flag;

protected:
	UPROPERTY(EditAnywhere, Category = "AZ GameStateMachine")
	class TSubclassOf<class UAZStateMachine> state_machine_class;
	UPROPERTY() UAZStateMachine* state_machine;

private:
	bool is_started = false;
};
