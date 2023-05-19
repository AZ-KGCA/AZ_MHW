// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
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
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTakeDamageSignature, float, damage, FAttackInfo, attack_info, class AController*, instigator /* Actor*, DamageCauser*/ );

public:
	FOnTakeDamageSignature OnTakeDamage;
	
protected:
	// Damage functions
	UFUNCTION(BlueprintNativeEvent) float ApplyDamage(AActor* damaged_actor, const FHitResult& hit_result, AController* event_instigator, FAttackInfo attack_info);
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result, AController* event_instigator, FAttackInfo attack_info);

public:
	virtual float ProcessDamage(const FHitResult& hit_result, AController* event_instigator, FAttackInfo attack_info, float applied_damage);
};
