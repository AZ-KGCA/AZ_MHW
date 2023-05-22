// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AZObjectMgr_Server.generated.h"

class AAZCharacter;
class AAZPlayer;
class AAZMonster;


UCLASS()
class AZ_MHW_API UAZObjectMgr_Server : public UObject
{
	GENERATED_BODY()
	UAZObjectMgr_Server();
	virtual void BeginDestroy() override;
	
public:
	void AddObject(AAZCharacter* character);
	bool RemoveObject(int32 object_serial);
	bool SpawnMonster(int32 monster_id, FVector spawn_location = FVector::ZeroVector, FRotator spawn_rotation = FRotator::ZeroRotator);
	
	AAZCharacter* GetObject(int32 object_serial);
	AAZMonster* GetMonster(int32 object_serial);
	AAZPlayer* GetPlayer(int32 object_serial);
	
private:
	UPROPERTY() TMap<int32, TObjectPtr<AAZCharacter>> object_map_;
	static int32 object_serial_;
};
