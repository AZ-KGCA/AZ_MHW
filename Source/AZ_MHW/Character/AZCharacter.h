#pragma once
#include <CoreMinimal.h>
#include <GenericTeamAgentInterface.h>
#include <GameFramework/Character.h>
#include "AZ_MHW/CommonSource/AZEnum.h"
#include <GameFramework/Character.h>
#include "AZCharacter.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration

#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion
/**
* PlayerController나 AIController가 빙의가능(조종)하고, 여러가지 향상된 폰
* Actor-> Pawn(빙의가능)-> Character(기본구현 및 움직임 개선)
* AZ_ Player, NPC, Enemy?Monster? Base Clas
*/
UCLASS(Abstract)
class AZ_MHW_API AAZCharacter : public ACharacter, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AAZCharacter();
#pragma region Inherited function
protected:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
#pragma endregion
public:
	//Client + Server
	//virtual void NotifyControllerChanged() override;	

	//virtual void NotifyAnimState??
	//현재 위치
	//현재 시선방향
	//현재 상태
	//현재 모션진행
public:
	//Server
	//virtual void NotifyActorBeginOverlap(AActor* Other) override;
	//virtual void NotifyActorEndOverlap(AActor* Other) override;
	//virtual void NotifyHit(class UPrimitiveComponent* MyComp, AActor* Other, class UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit) override;
	//이벤트 충돌
	//이벤트 데미지
public:
	//Client
	//ReciveMsg
	//Int32, AnimationState(누가, 어떤상태) X번이 피격상태, N번이 가드상태 등 
	//int32, DB Result(무엇이 몇임)ex 포션이 3개가됨. 체력이 X가됨, 디버프가 생김 등

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