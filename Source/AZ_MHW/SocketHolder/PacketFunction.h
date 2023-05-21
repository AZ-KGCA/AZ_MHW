#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "PacketFunction.generated.h"


UCLASS()
class AZ_MHW_API UPacketFunction : public UObject
{
	GENERATED_BODY()

public:
	UPacketFunction();
	static void LoginResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed);
	static void SigninResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed);
};
