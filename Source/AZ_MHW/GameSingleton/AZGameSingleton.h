// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZGameSingleton.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZGameSingleton : public UObject
{
	GENERATED_BODY()
public:
	UAZGameSingleton();
	
private:
	static UAZGameSingleton* instance_;

public:
	UPROPERTY() class UAZTableMgr* table_mgr;
	UPROPERTY() class UAZMonsterMgr* monster_mgr;
	static UAZGameSingleton* instance();
	void Init();
};
