#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "TotalItemData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UTotalItemData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 count;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString value;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 warehouse_max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 pocket_max;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 init_count;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString type;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
