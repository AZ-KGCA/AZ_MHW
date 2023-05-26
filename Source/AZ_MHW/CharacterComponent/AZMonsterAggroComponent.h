// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZMonsterAggroComponent.generated.h"
enum class EMonsterBodyPartChangeType : uint8;
class AAZPlayer_Origin;
class AAZMonster;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZMonsterAggroComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Initialisers
	UAZMonsterAggroComponent();
	void Init();
	void ActivateSystem();
	void InactivateSystem();
	void Reset();
	
	// Setters
	void ForceSetBestTarget(AAZPlayer_Origin* character);
	void RemoveTarget(int32 player_serial);
	
	// Getters
	int32 GetTargetSerial();
	AAZPlayer_Origin* GetTargetRef();
	FVector GetTargetLocation();
	FVector GetRandomTargetLocation();
	float GetDistance2DToTarget();
	float GetAngle2DToTarget();

	// Updaters
	void IncreaseByDamage(int32 attacker_serial, int32 damage);
	void IncreaseByPartChange(int32 attacker_serial, EMonsterBodyPartChangeType change_type);
	void UpdateByRange();
	void UpdateByEnterCombat(int32 player_serial);
	
private:
	// Update aggro information
	void SortAggroInfo();
	void UpdateAggro();
	void UpdateAggroSpecific(int32 player_serial, int32 aggro_point, FString update_reason);
	TPair<int32, int32>* FindByKey(int32 player_serial);
	
private:
	TWeakObjectPtr<AAZMonster> owner_;

	int32 target_serial_;
	TWeakObjectPtr<AAZPlayer_Origin> target_ref_;

	FTimerHandle update_timer_handle_;
	float update_rate_;
	float percept_range_;

	// Aggro information storing <object_serial/aggro_point>
	TArray<TPair<int32, int32>> aggro_info_;
};