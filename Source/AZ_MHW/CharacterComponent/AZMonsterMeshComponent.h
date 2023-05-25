// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZMonsterMeshComponent.generated.h"

enum class EMonsterBodyPart : uint8;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZMonsterMeshComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAZMonsterMeshComponent();
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;
	
	// Property Initialisers
	void InitializeMeshVisibilities();

protected:
	// Property Initialisers
	void SetUpDynamicMaterials();
	void SetUpBodyPartMaterialMaps();
	
	// Body Mesh Material Opacity Setters
	void SetMaterialVisibility(uint8 material_idx, bool is_visible);
	void SetMaterialVisibility(FName slot_name, bool is_visible);

	// Responders to body state changes
	UFUNCTION() void OnBodyPartWounded(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartWoundHealed(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartBroken(EMonsterBodyPart body_part);
	UFUNCTION() void OnBodyPartSevered(EMonsterBodyPart body_part);
	UFUNCTION() void OnDeath();

	UFUNCTION() void CloseEyes(bool should_close);
	UFUNCTION() void BlinkEyes();
	
private:
	TWeakObjectPtr<class AAZMonster> owner_;
	TWeakObjectPtr<USkeletalMeshComponent> mesh_;

	// material maps
	UPROPERTY(VisibleAnywhere, Category = "AZ | Material")
	TMap<EMonsterBodyPart, int32> mesh_material_indices_default_;

	UPROPERTY(VisibleAnywhere, Category = "AZ | Material")
	TMap<EMonsterBodyPart, int32> mesh_material_indices_wounded_;

	UPROPERTY(VisibleAnywhere, Category = "AZ | Material")
	TMap<EMonsterBodyPart, int32> mesh_material_indices_cutsurface_;

	// eye blink
	UPROPERTY(VisibleAnywhere) bool eyes_closed_;
	FTimerHandle blink_eye_timer_handle_;
};
