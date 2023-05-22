// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AZObjectMgr_Client.generated.h"

class AAZCharacter;
class AAZPlayer;
class AAZMonster_Client;


UCLASS()
class AZ_MHW_API UAZObjectMgr_Client : public UObject
{
	GENERATED_BODY()
	UAZObjectMgr_Client();
	virtual void BeginDestroy() override;
	
public:
	bool AddObject(int32 object_serial, AAZCharacter* character);
	bool RemoveObject(int32 object_serial);
	bool SpawnMonster(int32 object_serial, int32 monster_id, FVector spawn_location, FRotator spawn_rotation);
	
	AAZCharacter* GetObject(int32 object_serial);
	AAZMonster_Client* GetMonster(int32 object_serial);
	AAZPlayer* GetPlayer(int32 object_serial);
	
private:
	UPROPERTY() TMap<int32, TObjectPtr<AAZCharacter>> object_map_;
};
