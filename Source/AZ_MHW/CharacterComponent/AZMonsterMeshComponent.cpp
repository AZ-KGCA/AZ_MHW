// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent.h"

#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/Util/AZUtility.h"

// Sets default values for this component's properties
UAZMonsterMeshComponent::UAZMonsterMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UAZMonsterMeshComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMeshComponent] Invalid owner actor!"));
	}
	
	SetUpBodyPartMaterialMaps();
	InitializeRuntimeValues();
}

void UAZMonsterMeshComponent::BeginPlay()
{
	Super::BeginPlay();
	if (!owner_.IsValid()) return;
	if (owner_->IsABoss() && mesh_material_indices_default_.IsEmpty())
	{
		UE_LOG(AZMonster, Warning, TEXT("[AZMonsterMeshComponent] Mesh material indices not set for a boss monster!"));
	}
}

void UAZMonsterMeshComponent::SetUpBodyPartMaterialMaps()
{
	if (!owner_.IsValid()) return;
	if (!owner_->GetMesh()) return;
	if (!owner_->IsABoss()) return;

	int32 material_idx = 0;
	TArray<FName> material_slot_names = owner_->GetMesh()->GetMaterialSlotNames();
	for (FName slot_name : material_slot_names)
	{
		// Split the material slot name into body part name and mesh type
		FString slot_name_str = slot_name.ToString();
		FString body_part_str, mesh_type_str;
		if (!slot_name_str.Split(TEXT("_"), &body_part_str, &mesh_type_str))
		{
			UE_LOG(AZMonster, Warning, TEXT("[AZMonsterMeshComponent] Material slot name <%s> is in incorrect format"), *slot_name_str);
			material_idx++;
			continue;
		}

		// Add the body part name and material index to the respective material map
		if (mesh_type_str.Equals("Default", ESearchCase::IgnoreCase))
		{
			mesh_material_indices_default_.Add(UAZUtility::StringToEnum<EMonsterBodyPart>(body_part_str), material_idx);
		}
		else if (mesh_type_str.Equals("Wounded", ESearchCase::IgnoreCase))
		{
			mesh_material_indices_wounded_.Add(UAZUtility::StringToEnum<EMonsterBodyPart>(body_part_str), material_idx);
		}
		material_idx++;
	}
}

void UAZMonsterMeshComponent::InitializeRuntimeValues()
{
	if (!owner_.IsValid()) return;
	
	// Set up initial material opacities
	// Hide all damaged meshes
	for (const auto material_idx_pair : mesh_material_indices_wounded_)
	{
		SetMaterialVisibility(material_idx_pair.Value, false);
	}
	
	// Hide eyelid mesh by default
	if (const auto eyelid_material_idx = mesh_material_indices_default_.Find(EMonsterBodyPart::Eyelid))
	{
		SetMaterialVisibility(*eyelid_material_idx, false);
	}
	
	// Hide tail cut surface
	const int32 tail_cutsurface_material_idx = owner_->GetMesh()->GetMaterialIndex(FName(TEXT("Tail_CutSurface")));
	if (tail_cutsurface_material_idx != INDEX_NONE)
	{
		SetMaterialVisibility(tail_cutsurface_material_idx, false);
	}
}

/* Creates a dynamic material instance and set its opacity value */
void UAZMonsterMeshComponent::SetMaterialVisibility(uint8 material_idx, bool is_visible)
{
	if (!owner_.IsValid()) return;
	UMaterialInterface* parent_material = owner_->GetMesh()->GetMaterial(material_idx);
	UMaterialInstanceDynamic* dynamic_material = UMaterialInstanceDynamic::Create(parent_material, this);
	dynamic_material->SetScalarParameterValue("Opacity", is_visible == 1 ? 1.0f : 0.0f);
	owner_->GetMesh()->SetMaterial(material_idx, dynamic_material);
}

void UAZMonsterMeshComponent::OnBodyPartWounded(EMonsterBodyPart body_part)
{
	if (!owner_.IsValid()) return;
	if (const auto material_idx = mesh_material_indices_wounded_.Find(body_part))
	{
		SetMaterialVisibility(*material_idx, true);
		// TODO Save somewhere that the part is wounded
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMeshComponent] %s is not found in the damaged material map!"), *UAZUtility::EnumToString(body_part));
	}
}

void UAZMonsterMeshComponent::OnBodyPartBroken(EMonsterBodyPart body_part)
{
	if (!owner_.IsValid()) return;
	if (const auto material_idx = mesh_material_indices_default_.Find(body_part))
	{
		SetMaterialVisibility(*material_idx, false);
		// TODO Save somewhere that the part is broken
	}
	else
	{
		UE_LOG(AZMonster, Error, TEXT("[AZMonsterMeshComponent] %s is not found in the default material map!"), *UAZUtility::EnumToString(body_part));
	}
}
