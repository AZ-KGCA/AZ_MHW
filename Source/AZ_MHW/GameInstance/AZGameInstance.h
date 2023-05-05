// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "AZ_MHW/GameInstance/AZGameInstanceData.h"
#include "AZGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPROPERTY() class UAZGameConfig* game_config;
	UPROPERTY() class UAZSaveData* save_data;
	UPROPERTY() class UAZMsgHandler* msg_handler;
	UPROPERTY() class UAZLoginMgr* login_mgr;
	UPROPERTY() class UAZMapMgr* map_mgr;
	UPROPERTY() class UAZHUDDataMgr* hud_data_mgr;
	UPROPERTY() class UAZGameOption* game_option;

	UPROPERTY() FMapChannelInfo map_channel_info;

public:
	UAZGameInstance();
	virtual ~UAZGameInstance();

	virtual void Init() override;
	virtual void Shutdown() override;

	void RestMgr();
	void LobbyEnterAck();
	void PlayerSelectAck();

	void CreateSocketHolder();
	void DestroySocketHolder();
	void InitSocketOnMapLoad();

	UFUNCTION(BlueprintCallable, Category = "AZ") 
	class AAZHUD* GetHUD();
	//UFUNCTION(BlueprintCallable, Category = "AZ")
	//class AAZHUD_InGame* GetHUDInGame();

	UFUNCTION(BlueprintCallable, Category = "AZ")
	class AAZGameMode* GetGameMode();

	//UFUNCTION(BlueprintCallable, Category = "AZ")
	//class AAZGameMode_InGame* GetInGameMode();

	class UAZGameState* GetCurGameState();

	// FIXME merged need edit
	UFUNCTION(BlueprintCallable, Category = "AZ")
	APlayerController* GetPlayerController();
	// FIXME merged need edit
	ACharacter* GetPlayer();

	UFUNCTION()
	virtual void BeginLoadingScreen(const FString& map_name);

	UFUNCTION()
	virtual void EndLoadingScreen(UWorld* in_loaded_world);

public:
	void OnGameMsg(struct FAZGameMsg* game_msg);

private:
	void SendLogoutCmd();
	void AddNewSingleton(class UAZSingletonObject* mgr);

private:
	UPROPERTY() TArray<class UAZSingletonObject*> mgrs;
};

class UGameInstanceProxy
{
public:
	UGameInstanceProxy() :
		game_instance_(nullptr)
	{};

	inline UAZGameInstance* operator->()
	{
		// GInst is changed often on the game thread when in PIE, accessing on any other thread is going to be a race condition
		// In general, the rendering thread should not dereference UObjects, unless there is a mechanism in place to make it safe
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

	inline const UAZGameInstance* operator->() const
	{
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

	inline UAZGameInstance& operator*()
	{
		//checkSlow(IsInGameThread());
		return *game_instance_;
	}

	inline const UAZGameInstance& operator*() const
	{
		//checkSlow(IsInGameThread());
		return *game_instance_;
	}

	inline UGameInstanceProxy& operator=(UAZGameInstance* InWorld)
	{
		game_instance_ = InWorld;
		return *this;
	}

	inline UGameInstanceProxy& operator=(const UGameInstanceProxy& InProxy)
	{
		game_instance_ = InProxy.game_instance_;
		return *this;
	}

	inline bool operator==(const UGameInstanceProxy& Other) const
	{
		return game_instance_ == Other.game_instance_;
	}

	inline operator UAZGameInstance* () const
	{
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

	inline UAZGameInstance* GetReference()
	{
		//checkSlow(IsInGameThread());
		return game_instance_;
	}

private:
	UAZGameInstance* game_instance_;
};

extern AZ_MHW_API class UGameInstanceProxy AZGameInstance;
