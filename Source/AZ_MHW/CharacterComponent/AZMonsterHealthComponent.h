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
	void InitializeRuntimeValues();

	// Property Getters
	float GetHealthRatio() const;
	float GetStaminaRatio() const;
	bool IsWounded(EMonsterBodyPart body_part);
	bool IsPendingKill() const;

	// Escape Functionalities
	//TODO bool CanEscape() const;

	// Damage Agent Overrides
	float ApplyDamage(AActor* damaged_actor, const FHitResult& hit_result, AController* event_instigator, FAttackInfo attack_info);
	float ProcessDamage(const FHitResult& hit_result, AController* instigator, FAttackInfo attack_info, float applied_damage);
	UFUNCTION() void PostProcessDamage(float total_damage, FAttackInfo attack_info, AController* damage_instigator);

protected:
	// Damage functions
	bool IsReceivedAttackValid(EDamageType damage_type, EMonsterBodyPart damaged_part);
	void ApplyDamageToBodyPart(EDamageType damage_type, EMonsterBodyPart damaged_part, float damage_amount);
	void CheckBeWounded(EMonsterBodyPart damaged_part, float damage_amount);
	void CheckBeBroken(EMonsterBodyPart damaged_part, float damage_amount);
	void CheckBeSevered(EMonsterBodyPart damaged_part, float damage_amount);
	void ReduceHealth(float amount);

	// Broadcasted via owner delegate
	UFUNCTION() void OnBodyPartWounded(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartBroken(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartSevered(EMonsterBodyPart body_part);
	UFUNCTION() void OnDeath();
	
protected:
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	
private:
	TWeakObjectPtr<class AAZMonster> owner_;
	
	UPROPERTY(VisibleAnywhere, Category = "AZ | Health") int32 base_hp_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Stamina") int32 base_stamina_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Stamina") int32 tired_duration_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Health") FBossEscapeStats escape_stats_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") FBossWeaknessStats weakness_stats_;
	UPROPERTY(EditAnywhere, Category = "AZ | Damage") TMap<EMonsterBodyPart, FBossBodyPartState> body_part_states_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Damage") float tenderised_damage_multiplier_;
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Current State") int32 current_hp_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Current State") int32 current_stamina_;
	UPROPERTY(VisibleAnywhere, Category = "AZ | Current State") int32 current_num_escapes_;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AZ | Current State") FBossBodyCondition body_condition_; // currently not in use

};
