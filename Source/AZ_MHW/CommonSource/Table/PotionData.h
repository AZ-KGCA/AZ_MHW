#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/TableBase.h"
#include "PotionData.generated.h"

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UPotionData : public UTableBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly) int32 id;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) FString buff_type;
	UPROPERTY(EditAnywhere, BlueprintReadOnly) bool usable;

public:
	virtual void Load(TMap<FString, FString>& data) final;
};
