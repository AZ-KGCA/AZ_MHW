// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayer.h"
#include "AZPlayer_Origin.generated.h"

/**
 * 서버에서 플레이되는 원본 캐릭터를 표현하는 객체
 * 해당 객체의 상태가 실제 유저 캐릭터 객체의 상태다.
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayer_Origin : public AAZPlayer
{
	GENERATED_BODY()

public:
	AAZPlayer_Origin();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void Tick(float delta_seconds) override;
	/** */
	virtual void BeginDestroy() override;
	
	/** */
	//virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component) override;
	/** */
	//virtual void PossessedBy(AController* new_controller) override;
#pragma endregion


	
#pragma region Damage Interface
	// 해당 결과가 유저에게 전달
protected:
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result, AController* event_instigator, const FAttackInfo& attack_info) override;
	virtual float ProcessDamage(const FHitResult& hit_result, AController* event_instigator, const FAttackInfo& attack_info, float applied_damage) override;
#pragma endregion 
};
