// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AZDamageAgentInterface.generated.h"

UINTERFACE(Blueprintable)
class UAZDamageAgentInterface : public UInterface
{
	GENERATED_BODY()
};

class AZ_MHW_API IAZDamageAgentInterface
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamageSignature, float, damage, const class UDamageType*, damage_type, class AController*, instigator /* Actor*, DamageCauser*/ );

public:
	FOnTakeDamageSignature OnTakeDamage;
	
public:
	// Damage functions
	// If multiple damage types need to be handled in the future, need to change parameters accordingly
	// TEMP: Declared blueprintNativeEvent for bp debugging purposes
	UFUNCTION(BlueprintNativeEvent)
	float ApplyDamage(AActor* damaged_actor, const FHitResult& hit_result,
	                         AController* event_instigator, TSubclassOf<UDamageType> damage_type_class, float base_damage);
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result,
							 AController* event_instigator, TSubclassOf<UDamageType> damage_type_class, float base_damage);
	virtual float ProcessDamage(const FHitResult& hit_result, AController* event_instigator, TSubclassOf<UDamageType> damage_type_class, float applied_damage);
};
