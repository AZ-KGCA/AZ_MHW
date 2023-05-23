// Copyright Team AZ. All Rights Reserved.

#include "AZ_MHW/Character/AZCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

AAZCharacter::AAZCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
	SetGenericTeamId(uint8(EObjectType::None));
}

void AAZCharacter::SetGenericTeamId(const FGenericTeamId& team_id)
{
	team_id_ = team_id;
}

FGenericTeamId AAZCharacter::GetGenericTeamId() const
{
	return team_id_;
}

void AAZCharacter::AnimNotify_EndOfAction()
{
	// empty in base class
}

void AAZCharacter::AnimNotify_JumpToAnimation(FName next_animation_name, FName next_montage_section_name)
{
	// empty in base class
}

void AAZCharacter::AnimNotify_SetMovementMode(EMovementMode movement_mode)
{
	// empty in base class
}

void AAZCharacter::AnimNotify_DoSphereTrace(FName socket_name, float radius, EEffectDurationType duration_type, float duration)
{
	// empty in base class
}

float AAZCharacter::GetRelativeAngleToLocation(const FVector& target_location) const
{
	const FRotator rotator = UKismetMathLibrary::FindRelativeLookAtRotation(GetActorTransform(), target_location);
	if (rotator.IsNearlyZero())
		return 0.0f;
	else
		return rotator.Yaw;
}

float AAZCharacter::GetDistance2DToLocation(const FVector& target_location) const
{
	return FVector2D::Distance(FVector2D(GetActorLocation()), FVector2D(target_location));
}