// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "Character/AZCharacter.h"
#include "AZPlayer.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration
class AAZSocketActor;
#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion
/**
 * 모든 플레이어들은 동일하게 움직이고, 동일한 구조를 지녓다.
 * 플레이어블과 리모트의 차이는 직접 명령을 받느냐 아니냐의 차이
 * 리소스의 포인터만 들고있다. 
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
	virtual void PostInitProperties() override;
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void PossessedBy(AController* NewController) override;
	/** */
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** */
	virtual  void BeginDestroy() override;
#pragma endregion
public:
	//모든 플레이어 캐릭터(폰)가 가지는 게임요소들
	//UCapsuleComponent//SceneComponent(Physics)
	//UCharacterMovement//MovementComponent
	//SkeletalMeshComponent//Animation BaseBone

	//UPROPERTY()
	//TMap<FName, USkeletalMeshComponent*> Parts;
	UPROPERTY() USkeletalMeshComponent* Head;
	UPROPERTY() USkeletalMeshComponent* Hair;
	UPROPERTY() USkeletalMeshComponent* Face;
	UPROPERTY() USkeletalMeshComponent* Body;
	UPROPERTY() USkeletalMeshComponent* Arm;
	UPROPERTY() USkeletalMeshComponent* Waist;
	UPROPERTY() USkeletalMeshComponent* Leg;

	UPROPERTY() AAZSocketActor* FirstSocket;
	UPROPERTY() AAZSocketActor* SecondSocket;
	
	/** playerState의 CharacterEquipState 변경후 호출하여 갱신*/
	UFUNCTION() void SetSKMeshParts();
	/** bForceUpdate: 이미 붙어 있는 컴포넌트도 또 호출할 것인가 */
	UFUNCTION() void CombineSKMeshParts(bool bForceUpdate);
	/** playerState의 CharacterEquipState 변경후 호출하여 갱신*/
	UFUNCTION() void SetSKMeshSocket();
	
	//Cache
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> BodyAnimInstanceClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> FaceAnimInstanceClass;
};
