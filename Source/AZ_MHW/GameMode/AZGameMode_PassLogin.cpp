// Copyright Team AZ. All Rights Reserved.


#include "AZGameMode_PassLogin.h"

#include "GameInstance/AZGameInstance.h"
#include "GameInstance/Packet.h"
#include "Kismet/GameplayStatics.h"

AAZGameMode_PassLogin::AAZGameMode_PassLogin()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAZGameMode_PassLogin::BeginPlay()
{
	Super::BeginPlay();
	
	FString id = "p";
	FString pw = "j";
	
	LOGIN_REQUEST_PACKET login_send_packet;
	login_send_packet.packet_id = (int)PACKET_ID::LOGIN_REQUEST;
	strcpy_s(login_send_packet.user_id, sizeof(login_send_packet.user_id), TCHAR_TO_ANSI(*id));
	strcpy_s(login_send_packet.user_pw, sizeof(login_send_packet.user_pw), TCHAR_TO_ANSI(*pw));
	login_send_packet.packet_length = sizeof(login_send_packet);
	AZGameInstance->Server_Packet_Send((char*)&login_send_packet, login_send_packet.packet_length);
	
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("PCUnitTestMap"),true,"?game=/Game/AZ/GameMode/BP_DevInGame.BP_DevInGame_C");
}



