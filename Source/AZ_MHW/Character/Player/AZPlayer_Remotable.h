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
	
	// Damage Processing
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result,
	                                         AController* event_instigator, TSubclassOf<UDamageType> damage_type_class,
	                                         float base_damage) override;
};
