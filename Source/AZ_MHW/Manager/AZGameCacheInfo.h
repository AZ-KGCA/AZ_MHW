// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/Manager/AZInventoryManager.h"
#include "AZGameCacheInfo.generated.h"

/**
 * 
 */

USTRUCT()
struct FCharacterSimpleInfo
{
	GENERATED_BODY()

	UPROPERTY() FString character_name_;
	UPROPERTY() FVector3d hair_color_;
	UPROPERTY() int32 hair_id_;
};

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZGameCacheInfo : public UObject
{
	GENERATED_BODY()

public:
	UAZGameCacheInfo();
	void Init();

private:
	static int32 alloc_character_index_;
	TMap<uint32/*id_hash*/, FString> id_check_;//해시코드 일치 시 중복아이디로 체크하기

	TMap<uint32/*id_hash*/, TArray<FCharacterSimpleInfo>> character_info_; // 캐릭터 정보
	UPROPERTY() TMap<int32/*character_index*/, class UAZInventoryManager*> character_inventory_;

public:
	// 공용 함수
	uint32 GetIDHashCode(FString& id);
	bool GetID(int32 id_hash, FString& out_id);
	TArray<FCharacterSimpleInfo> GetCharacterSimpleInfo(uint32 id);
	UAZInventoryManager* GetInventoryManager(int32 character_index);

	// 클라 전용
public:
	FString id_;
	int32 current_character_index_;


	// 서버
public:
	bool SignupRequest(FString& id);
};
