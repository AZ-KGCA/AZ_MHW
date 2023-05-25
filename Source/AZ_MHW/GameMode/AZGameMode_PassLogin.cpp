// Copyright Team AZ. All Rights Reserved.


#include "AZGameMode_PassLogin.h"

#include "GameInstance/AZGameInstance.h"
#include "GameInstance/Packet.h"
#include "Kismet/GameplayStatics.h"
#include "SocketHolder/AZSocketHolder.h"

AAZGameMode_PassLogin::AAZGameMode_PassLogin()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAZGameMode_PassLogin::BeginPlay()
{
	Super::BeginPlay();
	AZGameInstance->Server_Connect();
	FString id = "p";
	FString pw = "j";
	
	CS_LOGIN_SIGNIN_REQ login_send_packet;
	login_send_packet.packet_id = (int)PACKET_ID::CS_LOGIN_SIGNIN_REQ;
	FCStringAnsi::Strncpy(login_send_packet.user_id, TCHAR_TO_ANSI(*id), sizeof(login_send_packet.user_id) - 1);
	FCStringAnsi::Strncpy(login_send_packet.user_pw, TCHAR_TO_ANSI(*pw), sizeof(login_send_packet.user_pw) - 1);
	login_send_packet.packet_length = sizeof(login_send_packet);
	
	//AZGameInstance->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&login_send_packet, login_send_packet.packet_length);
	AZGameInstance->Server_Packet_Send((char*)&login_send_packet, login_send_packet.packet_length);
	//AZGameInstance->BroadCastSendPacketFunc
	//AZGameInstance->SendPacketFunc
	//UGameplayStatics::OpenLevel(GetWorld(), TEXT("PCUnitTestMap"),true,"?game=/Game/AZ/GameMode/BP_DevInGame.BP_DevInGame_C");
}



