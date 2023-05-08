// Fill out your copyright notice in the Description page of Project Settings.


#include "Chatting.h"
#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "AZGameInstance.h"

void UChatting::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("[NativeConstruct]\n"));

	ChatHistory = Cast<UScrollBox>(GetWidgetFromName(TEXT("ChatHistory")));
	ChatMsgBlock = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());

	teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	if (teemo_game_instance->client_connect != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UChatting if]\n"));
		teemo_game_instance->client_connect->Fuc_boradcast_success.BindUFunction(this, FName("ChatHistory_Msg"));
	}
}

UChatting::UChatting(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("[UChatting]\n"));
}

void UChatting::ChatMsgSend(FString msg)
{
	Login_Send_Packet login_send_packet;
	login_send_packet.packet_id = (int)CLIENT_PACKET_ID::CHAT_SEND_REQUEST;
	strcpy(login_send_packet.user_id, FStringToCharArray(*msg));
	login_send_packet.packet_length = sizeof(login_send_packet);

	UE_LOG(LogTemp, Warning, TEXT("[ChatMsgSend.cpp] pakcet_id : %d, pakcet_userid : %s \n"), login_send_packet.packet_id, *CharArrayToFString(login_send_packet.user_id));

	teemo_game_instance->PacketToServer((char*)&login_send_packet, login_send_packet.packet_length);
}


// Fstring to char
char* UChatting::FStringToCharArray(FString fString)
{
	std::string stdString(TCHAR_TO_UTF8(*fString));
	char* charArray = new char[stdString.length() + 1];
	strcpy(charArray, stdString.c_str());

	return charArray;
}

// char To FString
FString UChatting::CharArrayToFString(const char* CharArray)
{
	std::string StdString = CharArray;

	return FString(StdString.c_str());
}

void UChatting::ChatHistory_Msg(FString msg)
{
	UE_LOG(LogTemp, Warning, TEXT("ChatHistory_Msg / %s"), *msg)

		// TODO ä�� ��ε�ĳ��Ʈ ��������Ʈ �ް� ä��â�� �۾� �ִ°ǵ� ... �ڵ�� �����ϸ�ȵ�..
		//AddChatMessage(msg);
}

void UChatting::AddChatMessage(const FString& Message)
{
	// Text ������Ʈ�� �����ϰ�, ScrollBox�� �߰��Ѵ�.
	UTextBlock* NewTextBlock = NewObject<UTextBlock>(ChatHistory);
	NewTextBlock->SetText(FText::FromString(Message));

	ChatHistory->AddChild(NewTextBlock);
	ChatHistory->ScrollToEnd(); // ���� �ֱ� ä���� ���� ����, ��ũ���� ���� �Ʒ��� ������.
}
