// Copyright Team AZ. All Rights Reserved.

#pragma once
#include <GenericTeamAgentInterface.h>
#include <GameFramework/Character.h>
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZCharacter.generated.h"

UCLASS(Abstract)
class AZ_MHW_API AAZCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAZCharacter();
	
public:
	// Team Agent Interface Overrides
	virtual FGenericTeamId GetGenericTeamId() const override;
	
	// AnimNotify Handlers
	virtual void AnimNotify_EndOfAction();
	virtual void AnimNotify_JumpToAnimation(FName next_animation_name, FName next_montage_section_name);
	virtual void AnimNotify_SetMovementMode(EMovementMode movement_mode);
	virtual void AnimNotify_DoSphereTrace(FName socket_name, float radius, EEffectDurationType duration_type, float duration);

	// Others
	float GetRelativeAngleToLocation(const FVector& target_location) const;
	float GetDistance2DToLocation(const FVector& target_location) const;

public:
	FGenericTeamId team_id_;
};