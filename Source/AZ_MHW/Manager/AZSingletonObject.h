// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZSingletonObject.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AZ_MHW_API UAZSingletonObject : public UObject
{
	GENERATED_BODY()

public:
	virtual void Init();
	virtual void ResetMgr();
	virtual void LobbyEnterAck();
	virtual void PlayerSelectAck();
};
