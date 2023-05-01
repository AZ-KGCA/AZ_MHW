// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>

#include "AZPlayerAssetMgr.generated.h"
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
 * 
 */
UCLASS()
class AZ_MHW_API UAZPlayerAssetMgr : public UObject
{
	GENERATED_BODY()
public:
	UAZPlayerAssetMgr();
#pragma region Inherited function
protected:
	virtual void BeginDestroy() override;
#pragma endregion
public:
	//Load된
	UPROPERTY() TMap<FName,UAnimSequence*> AnimSequenceMap;
	UPROPERTY() TMap<FName,UBlendSpace*> AnimBlendSpaceMap;
	UPROPERTY() TMap<FName,UAnimMontage*> AnimMontageMap;
	
	UFUNCTION() UAnimSequence* GetSequence(FName Name);
	UFUNCTION() bool UnloadSequence(FName Name);
	UFUNCTION() UBlendSpace* GetBlendSpace(FName Name);
	UFUNCTION() bool UnloadBlendSpace(FName Name);
	UFUNCTION() UAnimMontage* GetMontage(FName Name);
	UFUNCTION() bool UnloadMontage(FName Name);

	
	UPROPERTY() TMap<FName, USkeletalMesh*> PlayerCharacterPartMap;
	
	UFUNCTION() USkeletalMesh* GetSkeletalMesh(FName Name);
	UFUNCTION() bool UnloadSkeletalMesh(FName Name);

	UFUNCTION() void SetCommandBitMaskMap(FName WP_Name);
	UPROPERTY() TMap<FName, uint32> CommandBitMaskMap;
	UFUNCTION() uint32 GetCommandBitMask(FName Name);

	//UPROPERTY() TMap<FName, TMap<int32,FName>> MaskMap;
};
