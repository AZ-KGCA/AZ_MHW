// Copyright Team AZ. All Rights Reserved.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <InputActionValue.h>
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "AZPlayer_Playable.generated.h"


class AAZPlayerState_Client;


/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayer_Playable : public AAZPlayer
{
	GENERATED_BODY()

public:
	AAZPlayer_Playable();
	
#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void Tick(float delta_seconds) override;
	/** */
	virtual void BeginDestroy() override;
	/** */
	virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component) override;
	/** */
	virtual void PossessedBy(AController* new_controller) override;
#pragma endregion
public:
	
	//환경 감지하기?
	//피격당함
	//지면없음
	//액션중
	//인터렉션가능
	//벽타기가능 등
	//박스 이용?(밀기,들기,당기기) 어떻게 해
	
	UFUNCTION() void AnimNotify_OnUseItem();

	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info) override;
};