// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/Interface/AZDamageAgentInterface.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Util/AZUtility.h"

float IAZDamageAgentInterface::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result, AController* event_instigator,
	TSubclassOf<UDamageType> damage_type_class, float base_damage)
{
	// Validity Checks
	if (!damaged_actor)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Damaged actor does not exist"));
		return 0.f;
	}
	IAZDamageAgentInterface* damaged_agent = Cast<IAZDamageAgentInterface>(damaged_actor);
	if (!damaged_agent)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Damaged actor does not inherit DamageAgentInterface"));
		return 0.f;
	}
	AAZCharacter* damaged_character = Cast<AAZCharacter>(damaged_actor);
	if (!damaged_character)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Damaged actor is not AAZCharacter"));
		return 0.f;
	}
	AAZCharacter* instigator_character = Cast<AAZCharacter>(event_instigator->GetPawn());
	if (!instigator_character)
	{
		UE_LOG(AZMonster, Error, TEXT("[IAZDamageAgentInterface] Instigator is not AAZCharacter"));
		return 0.f;
	}

	// Debug log
	UDamageType const* const damage_type_cdo = damage_type_class? damage_type_class->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	UE_LOG(AZMonster, Log, TEXT("[IAZDamageAgentInterface] ApplyDamage called by %s to %s (type: %s, base damage: %f)"),
		*instigator_character->GetName(), *damaged_character->GetName(), *damage_type_cdo->GetName(), base_damage);

	// Process Damage on damaged actor
	return damaged_agent->ProcessDamage(hit_result, event_instigator, damage_type_class, base_damage);
}

float IAZDamageAgentInterface::ProcessDamage(const FHitResult& hit_result, AController* instigator,	TSubclassOf<UDamageType> damage_type_class, float applied_damage)
{
	// Broadcast Post Damage delegate bound functions
	UDamageType const* const damage_type_cdo = damage_type_class? damage_type_class->GetDefaultObject<UDamageType>() : GetDefault<UDamageType>();
	if (OnTakeDamage.IsBound())
	{
		OnTakeDamage.Broadcast(applied_damage, damage_type_cdo, instigator);
	}
	
	// Debug log
	const AAZCharacter* instigator_character = Cast<AAZCharacter>(instigator->GetPawn());
	const AAZCharacter* damaged_character = Cast<AAZCharacter>(this);
	if (!instigator_character || !damaged_character)
	{
		UE_LOG(AZMonster, Warning, TEXT("[IAZDamageAgentInterface] Incorrect character is inheriting DamageAgentInterface!"));
		return 0.f;
	}
	else
	{
		UE_LOG(AZMonster, Log, TEXT("[IAZDamageAgentInterface] ProcessDamage called by %s to %s (type: %s, base damage: %f)"),
		*instigator_character->GetName(), *damaged_character->GetName(), *damage_type_cdo->GetName(), applied_damage);
	}
	
	return applied_damage;
}