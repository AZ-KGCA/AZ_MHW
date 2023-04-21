// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/HUD/AZHUD_StartGame.h"

#include "AZ_MHW/Widget/StartGame/AZWidget_Splash.h"

void AAZHUD_StartGame::BeginPlay()
{
	Super::BeginPlay();

	// FIXME 병합시 삭제
	// CloseUI(EUIName::LHWidget_CheatCommand);

	UAZWidget_Splash* splash = OpenScene<UAZWidget_Splash>(EUIName::AZWidget_Splash);
	// 뒤로가기 버튼 끄기
	splash->SetIsDisableBackBtnExit(false);
}

void AAZHUD_StartGame::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD_StartGame::EndPlay(const EEndPlayReason::Type end_play_reason)
{
	Super::EndPlay(end_play_reason);
}
