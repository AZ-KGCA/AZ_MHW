// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZSocketHolder.generated.h"

UENUM(BlueprintType)
enum class ESocketResult : uint8
{
	Success,
	Failed,
	Error
};
/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZSocketHolder : public UObject
{
	GENERATED_BODY()
private:
	ESocketHolderType socket_holder_type_;

	FString connect_ip_;
	int32 connect_port_;

public:
	UAZSocketHolder();
	~UAZSocketHolder();

	void Init(ESocketHolderType socketType);
	void Disconnect();

	void Connect(FString ip, int32 port)
	{
		Disconnect();

		connect_ip_ = ip;
		connect_port_ = port;

		ESocketResult socket_result = ESocketResult::Success;
	}
};
