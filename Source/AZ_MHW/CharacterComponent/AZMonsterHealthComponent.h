// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZMonsterHealthComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZMonsterHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Property Initialisers
	UAZMonsterHealthComponent();
	virtual void InitializeComponent() override;
	void InitializeRuntimeValues();

	// Property Getters
	float GetHealthRatio() const;
	float GetStaminaRatio() const;
	//TODO bool CanEscape() const;

protected:
	UFUNCTION() void TakeDamage(AActor* damaged_actor, float damage_amount, const class UDamageType* damage_type, class AController* event_instigator, AActor* damage_causer);

private:
	UPROPERTY() TWeakObjectPtr<class AAZMonster> owner_;
	
	UPROPERTY(VisibleAnywhere, Category = "AZ | Health") int32 base_hp_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Stamina") int32 base_stamina_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Stamina") int32 tired_duration_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Health") FBossEscapeStats escape_stats_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") FBossWeaknessStats weakness_stats_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") FBossPartBreakStats part_break_stats_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") int32 sever_damage_tail_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") float tenderised_damage_multiplier_;

	UPROPERTY(VisibleAnywhere, Category = "AZ | Current State") int32 current_hp_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Current State") int32 current_stamina_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Current State") int32 current_num_escapes_;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Current State") FBossBodyCondition body_condition_;

	//TODO Add current damage per parts (sever, part break, stun, etc)
};
