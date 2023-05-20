// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZPlayer.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration
class AAZPlayerState;
class AAZSocketActor;
class UAZAnimInstance_Player;
#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion
/**
 * 모든 플레이어들은 동일하게 움직이고, 동일한 구조를 지녓다.
 * 플레이어블과 리모트의 차이는 직접 명령을 받느냐 아니냐의 차이
 * 화려한 센서다. 주변의 이벤트를 수집해 플레이어 스테이트에 전달하고,
 * 변화에 따른 캐릭터의 변동사항을 나타낸다.(메시변경, 이펙트 등)
 */
UCLASS(Abstract)
class AZ_MHW_API AAZPlayer : public AAZCharacter
{
	GENERATED_BODY()

public:
	AAZPlayer();
	
#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void Tick(float delta_seconds) override;
	/** */
	virtual  void BeginDestroy() override;
	/** */
	//virtual void PossessedBy(AController* new_controller) override;
	/** */
	//virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component) override;
	
#pragma endregion
public:
	//UPROPERTY() TMap<FName, USkeletalMeshComponent*> character_parts_map_;
	UPROPERTY() USkeletalMeshComponent* face_mesh_;
	UPROPERTY() USkeletalMeshComponent* head_mesh_;
	UPROPERTY() USkeletalMeshComponent* hair_mesh_;
	UPROPERTY() USkeletalMeshComponent* body_mesh_;
	UPROPERTY() USkeletalMeshComponent* arm_mesh_;
	UPROPERTY() USkeletalMeshComponent* waist_mesh_;
	UPROPERTY() USkeletalMeshComponent* leg_mesh_;
	UPROPERTY() TMap<FName, AAZSocketActor*> character_sockets_map_;
	
	//메시/////////////////////////////////////////////////////
	/** playerState의 CharacterEquipState 변경후 호출하여 갱신*/
	UFUNCTION() void SetSKMeshParts();
	/** bForceUpdate: 이미 붙어 있는 컴포넌트도 또 호출할 것인가*/
	UFUNCTION() void CombineSKMeshParts(bool is_force_update = true);
	/** playerState의 CharacterEquipState 변경후 호출하여 갱신*/
	UFUNCTION() void SetSKMeshSocket();
	/** 장비타입, 아이템아이디 부위별 변경기능*/
	UFUNCTION() void ChangeEquipmentMesh(int32 item_id);

	//소켓/////////////////////////////////////////////////////
	/** 소켓액터 추가*/
	UFUNCTION() void CreateSocketActor(FName new_socket_actor_name, FName in_socket_name);
	/** 소켓위치 변경*/
	UFUNCTION() void ChangeSocketSlot(FName socket_actor_name, FName in_socket_name);
	/** 소켓장비(메시) 변경*/
	UFUNCTION() void ChangeSocketMesh(FName socket_actor_name, int32 item_id);
	

	
#pragma region Damage Interface
protected:
	// Damage Processing
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable) float ApplyDamage(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info);
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info) override;
	virtual float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info) override;
	UFUNCTION() void PostProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info);
#pragma endregion 
};
