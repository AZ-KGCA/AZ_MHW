// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "AZPlayer_Remotable.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayer_Remotable : public AAZPlayer
{
	GENERATED_BODY()
	
public:
	AAZPlayer_Remotable();

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
	// Damage Processing
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info) override;
	virtual float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info) override;
};
