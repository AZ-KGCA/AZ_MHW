// Fill out your copyright notice in the Description page of Project Settings.

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
	UAZMonsterHealthComponent();

protected:
	virtual void BeginPlay() override;
	UFUNCTION() void TakeDamage(AActor* damaged_actor, float damage_amount, const class UDamageType* damage_type, class AController* event_instigator, AActor* damage_causer);
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health") int32 base_hp_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina") int32 base_stamina_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina") int32 tired_duration_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health") FBossEscapeStats escape_stats_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage") FBossWeaknessStats weakness_stats_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage") FBossPartBreakStats part_break_stats_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage") int32 sever_damage_tail_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Damage") float tenderised_damage_multiplier_;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Current State") int32 curr_hp_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Current State") int32 curr_stamina_;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Current State") int32 curr_num_escapes_;
};
