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
class AAZPlayerState_Client;
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
	//virtual void PossessedBy(AController* new_controller) override;
	/** */
	//virtual void SetupPlayerInputComponent(class UInputComponent* player_input_component) override;
	/** */
	virtual void Tick(float delta_seconds) override;
	/** */
	virtual  void BeginDestroy() override;
#pragma endregion
public:
	//각 Player_... Origin, Playable, Remotable 초기화 시점이 다르다.
	//Playable->빙의시 컨트롤러에서 초기화
	//Origin->생성시 컨르롤러에서 T맵에서 초기화
	//Remotable->생성시 컨르롤러에서 T맵에서 초기화
	UPROPERTY() AAZPlayerState_Client* player_character_state_;
	
	//UPROPERTY() TMap<FName, USkeletalMeshComponent*> character_parts_map_;
	UPROPERTY() USkeletalMeshComponent* face_mesh_;
	UPROPERTY() USkeletalMeshComponent* head_mesh_;
	UPROPERTY() USkeletalMeshComponent* hair_mesh_;
	UPROPERTY() USkeletalMeshComponent* body_mesh_;
	UPROPERTY() USkeletalMeshComponent* arm_mesh_;
	UPROPERTY() USkeletalMeshComponent* waist_mesh_;
	UPROPERTY() USkeletalMeshComponent* leg_mesh_;
	
	UPROPERTY() TMap<FName, AAZSocketActor*> character_sockets_map_;
	//playable, remotable만 필요한 것이지만 시간이 없다.
	UPROPERTY() USkeletalMeshComponent* face_fx_mesh_;
	UPROPERTY() USkeletalMeshComponent* head_fx_mesh_;
	UPROPERTY() USkeletalMeshComponent* hair_fx_mesh_;
	UPROPERTY() USkeletalMeshComponent* body_fx_mesh_;
	UPROPERTY() USkeletalMeshComponent* arm_fx_mesh_;
	UPROPERTY() USkeletalMeshComponent* waist_fx_mesh_;
	UPROPERTY() USkeletalMeshComponent* leg_fx_mesh_;
	
	/** 메시 파츠 갱신처리시 호출*/
	void SetSKMeshParts();
	/** 무기 소켓 갱신처리시 호출*/
	void SetSKMeshSocket();
	/** 바디 이펙트용 메시의 머테리얼 설정(TODO: 임시 고정 이펙트)*/
	//void SetMeshEfxMaterial(FString material_name_path);
	void SetSKMeshEfxMaterial();
	/** */
	void SetNiagaraEfx();
	/** 바디 이펙트 온 오프*/
	UFUNCTION(BlueprintCallable)
	void SetEnableSKMeshEfx(bool on_off);
	
	/** 소켓액터 추가 */
	void CreateSocketActor(FName new_socket_actor_name, FName in_socket_name);
	/** AnimNotify 호출. 소켓위치 변경 */
	void ChangeSocketSlot(FName socket_actor_name, FName in_socket_name);
	/** 컨트롤러 대리 호출. 소켓장비(메시) 변경 */
	void ChangeSocketMesh(FName socket_actor_name, int32 item_id);
	/** 컨트롤러 대리 호출. 장비타입, 아이템아이디 부위별 변경기능 */
	void ChangeEquipment(int32 item_id);

private:
	/** bForceUpdate: 이미 붙어 있는 컴포넌트도 또 호출할 것인가*/
	void CombineSKMeshParts(bool is_force_update = true);
	
#pragma region Damage Interface
public:
	virtual float ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info) override;
protected:
	virtual float ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info) override;
	/** 다이나믹 델리게이트에서 처리되기 때문에 UFUNCTION 처리*/
	UFUNCTION() virtual void PostProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info);
#pragma endregion
#pragma region 추후 추가해야할 Interface?

	//OnUseItem
	//OnGetItem
	//OnInteraction
	//OnChangeEquipment
	//OnCostState
	
#pragma endregion 

	UPROPERTY() class UAZGameInstance* game_instance_;
};
