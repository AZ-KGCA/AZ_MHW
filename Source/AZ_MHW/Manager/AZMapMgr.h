// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/Manager/AZSingletonObject.h"
#include "AZ_MHW/ClientMsg/AZMsgObject.h"
#include "AZMapMgr.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZMapMgr : public UAZSingletonObject, public FTickableGameObject, public IAZMsgObject
{
	GENERATED_BODY()

public:
	UAZMapMgr();
	virtual ~UAZMapMgr();

	virtual void Init() override;
	virtual void LobbyEnterAck() override;
	virtual void Tick(float delta_time) override;
	virtual void OnGameMsg(struct FAZGameMsg* new_msg) override;
	bool IsTickable() const override;
	bool IsTickableInEditor() const override;
	bool IsTickableWhenPaused() const override;
	virtual TStatId GetStatId() const override { return TStatId(); }
	
};
