#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/GameInstance/Client_Packet.h"
#include "PacketFunction.generated.h"


UCLASS()
class AZ_MHW_API UPacketFunction : public UObject
{
	GENERATED_BODY()

public:
	UPacketFunction();
	static void LoginResponse(Login_Send_Packet* packet, bool is_successed);
	static void SigninResponse(Login_Send_Packet* packet, bool is_successed);
};
