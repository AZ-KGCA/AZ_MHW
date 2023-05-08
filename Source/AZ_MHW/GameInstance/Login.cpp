// Fill out your copyright notice in the Description page of Project Settings.


#include "Login.h"
#include "Client_Packet.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/WidgetLayoutLibrary.h"
#include "AZGameInstance.h"

void ULogin::NativeConstruct()
{
	Super::NativeConstruct();

	teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	if (teemo_game_instance->client_connect != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULogin NativeConstruct]\n"));
		teemo_game_instance->client_connect->Fuc_in_game_connect.BindUFunction(this, FName("InGameConnect"));
	}
}

FString ULogin::SetLoginInfo(FString id, FString pw)
{
	Login_Send_Packet login_send_packet;
	login_send_packet.packet_id = (int)CLIENT_PACKET_ID::LOGIN_REQUEST;
	strcpy(login_send_packet.user_id, TCHAR_TO_ANSI(*id));
	strcpy(login_send_packet.user_pw, TCHAR_TO_ANSI(*pw));
	login_send_packet.packet_length = sizeof(login_send_packet);

	login_id = id;
	login_pw = pw;

	UE_LOG(LogTemp, Warning, TEXT("[Login.cpp] pakcet_id : %d, pakcet_userid : %s\n"), login_send_packet.packet_id, login_send_packet.user_id);

	//teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	teemo_game_instance->PacketToServer((char*)&login_send_packet, login_send_packet.packet_length);

	// TODO SendPakcet 정상 로그인 성공시 OpenLevel
	//if (teemo_game_instance->LoginRecord(login_id, login_pw))
	//{
	//	UWidgetLayoutLibrary::RemoveAllWidgets(this);
	//	UGameplayStatics::OpenLevel(this, FName("Basic_Map"));
	//}	

	return login_id;

}

void ULogin::PacketSend()
{
	teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	char UserID[71];
	//char UserPw[33];
	char HeaderSize[5] = "1001";

	strcat(UserID, HeaderSize);

	UE_LOG(LogTemp, Warning, TEXT("PacketSend : %s\n"), UserID);
	sizeof(UserID);

	UINT32 clinet_index = 19;
	char P_send_data[10] = "minsuhong";
	teemo_game_instance->iocp_net_server_.SendMsg(clinet_index, sizeof(UserID), UserID);
	//teemo_game_instance->
}

void ULogin::ServerConnect()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerConnect\n"), );

	//teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	teemo_game_instance->ServerConnect();
}

void ULogin::InGameConnect()
{
	UE_LOG(LogTemp, Warning, TEXT("InGameConnect\n"));

	login_to_ingame = true;

	//UWidgetLayoutLibrary::RemoveAllWidgets(this);
	//UGameplayStatics::OpenLevel(this, FName("Basic_Map"));
}