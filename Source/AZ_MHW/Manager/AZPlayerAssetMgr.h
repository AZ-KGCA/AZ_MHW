// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
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
 * GameInstance에 넣으면 생성시점이 Play이후이므로 실행중이지 않은 Editor에서 호출시 터진다.
 * GameSingleton을 사용해야 한다?
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
	UPROPERTY() TMap<FName, UAnimSequence*> anim_sequence_map_;
	UPROPERTY() TMap<FName, UAnimMontage*> anim_montage_map_;
	UPROPERTY() TMap<int32, USkeletalMesh*> player_character_part_map_;//끝
	UPROPERTY() TMap<FName, uint32> command_bit_mask_map_;
	
	UFUNCTION() UAnimSequence* GetSequence(FName name);
	UFUNCTION() bool UnloadSequence(FName name);
	
	UFUNCTION() UAnimMontage* GetMontage(FName name);
	UFUNCTION() bool UnloadMontage(FName name);

	UFUNCTION() USkeletalMesh* GetSkeletalMesh(int32 item_id);
	UFUNCTION() bool UnloadSkeletalMesh(int32 item_id);

	UFUNCTION() void SetCommandBitMaskMap(int32 weapon_type);
	UFUNCTION() uint32 GetCommandBitMask(FName name);

	//UPROPERTY() TMap<FName, TMap<int32,FName>> MaskMap;

};
