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
class AAZPlayerState;
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
UCLASS()
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
	//virtual void PossessedBy(AController* NewController) override;
	/** */
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	/** */
	virtual  void BeginDestroy() override;
#pragma endregion
public:
	//모든 플레이어 캐릭터가 가지는 게임요소들
	//UCapsuleComponent//SceneComponent(Physics)
	//UCharacterMovement//MovementComponent
	//SkeletalMeshComponent* //NoneVisible Animation BaseBone

	UPROPERTY() USkeletalMeshComponent* Head;
	UPROPERTY() USkeletalMeshComponent* Hair;
	UPROPERTY() USkeletalMeshComponent* Face;
	UPROPERTY() USkeletalMeshComponent* Body;
	UPROPERTY() USkeletalMeshComponent* Arm;
	UPROPERTY() USkeletalMeshComponent* Waist;
	UPROPERTY() USkeletalMeshComponent* Leg;
	//UPROPERTY()
	//TMap<FName, USkeletalMeshComponent*> Parts;

	/** bForceUpdate이미 붙은 컴포넌트도 또 호출할 것인가
	 */
	UFUNCTION(BlueprintCallable)
	void SetCombineSKMeshParts(bool bForceUpdate);
	
	//일반상태->전투상태(무기종류별)
	//IMC변경
	//GetMesh()->AnimInstance를 변경

	//임시
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> CommonAnimInstanceClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> WeaponAnimInstanceClass;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UAnimInstance> FaceAnimInstanceClass;
	//애니메이션이 변경되는 경우(일반이냐 무기냐)
	//void ChangeAnimInstance(FName AinmName);
	//TMap<FName,TSubclassOf<UAnimInstance> *> AnimationMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AAZPlayerState* AZPlayerState;
};
