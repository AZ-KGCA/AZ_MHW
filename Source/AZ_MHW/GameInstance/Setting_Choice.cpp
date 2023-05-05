// Fill out your copyright notice in the Description page of Project Settings.


#include "Setting_Choice.h"
#include <Kismet/GameplayStatics.h>
#include "Blueprint/WidgetLayoutLibrary.h"
#include "AZGameInstance.h"

void USetting_Choice::NativeConstruct()
{
	Super::NativeConstruct();

	teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());
}

void USetting_Choice::ServerStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Iocp_Odbc_Start\n"), );

	//teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	teemo_game_instance->IocpOdbcOpen();
}

void USetting_Choice::ClientStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Client_Start\n"), );

	//teemo_game_instance = Cast<UAZGameInstance>(GetGameInstance());

	teemo_game_instance->ServerConnect();
}