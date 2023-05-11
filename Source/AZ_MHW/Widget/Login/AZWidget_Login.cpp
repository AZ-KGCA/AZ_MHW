// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Login/AZWidget_Login.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
//#include "AZ_MHW/CommonSource/Define/"

void UAZWidget_Login::Init()
{
	Super::Init();

	AZ_PRINT_LOG_IF_FALSE(c_btn_login_);
	c_btn_login_->OnClicked.RemoveDynamic(this, &UAZWidget_Login::OnClicked_Login);
	c_btn_login_->OnClicked.AddDynamic(this, &UAZWidget_Login::OnClicked_Login);

	AZ_PRINT_LOG_IF_FALSE(c_btn_any_press_);
	c_btn_any_press_->OnClicked.RemoveDynamic(this, &UAZWidget_Login::OnTouchAnyPress);
	c_btn_any_press_->OnClicked.AddDynamic(this, &UAZWidget_Login::OnTouchAnyPress);

	AZ_PRINT_LOG_IF_FALSE(c_btn_close_);
	c_btn_close_->OnClicked.RemoveDynamic(this, &UAZWidget_Login::OnClicked_Close);
	c_btn_close_->OnClicked.AddDynamic(this, &UAZWidget_Login::OnClicked_Close);

	panel_state_[(int32)EPanelState::TouchScreen] = GetOwnWidget<UWidget>(FString(TEXT("panel_state_touch")));
	panel_state_[(int32)EPanelState::IDPasswordLogin] = GetOwnWidget<UWidget>(FString(TEXT("panel_state_id")));

	for (int32 i = 0; i < (int32)EPanelState::Max; ++i)
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[i]);
		panel_state_[i]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UAZWidget_Login::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);

	AZGameInstance->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::WaitingForTouch);
}

void UAZWidget_Login::SetLoginMode(ELogInMode login_mode)
{
	for (int32 i = 0; i < (int32)EPanelState::Max; ++i)
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[i]);
		panel_state_[i]->SetVisibility(ESlateVisibility::Collapsed);
	}

	switch (login_mode)
	{
	case ELogInMode::TouchConnect:
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[(int32)EPanelState::TouchScreen]);
		panel_state_[(int32)EPanelState::TouchScreen]->SetVisibility(ESlateVisibility::Visible);
		c_btn_any_press_->SetVisibility(ESlateVisibility::Visible);

	}break;
	case ELogInMode::IDPassword:
	{
		AZ_PRINT_LOG_IF_FALSE(panel_state_[(int32)EPanelState::IDPasswordLogin]);
		panel_state_[(int32)EPanelState::IDPasswordLogin]->SetVisibility(ESlateVisibility::Visible);
	}break;
	}
}

void UAZWidget_Login::Connect()
{
	if (AZGameInstance->login_mgr->GetSequence() > UAZLoginMgr::ESequence::ConnectLoginServer)
	{
		return;
	}


}

void UAZWidget_Login::OnTouchAnyPress()
{
 	c_btn_any_press_->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	UAZLoginMgr* login_mgr = AZGameInstance->login_mgr;
	if (login_mgr != nullptr)
	{
		// FIXME 로그인 준비 로직(2차 병합시 확인하기)
		login_mgr->ChangeSequence(UAZLoginMgr::ESequence::ConnectLoginServerReady);
	}
}

void UAZWidget_Login::OnClicked_Login()
{
	FString id = c_id_->GetText().ToString();
	FString password = c_pass_->GetText().ToString();
	AZGameInstance->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::ConnectLoginServer);
}

void UAZWidget_Login::OnClicked_Close()
{
	AZGameInstance->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::GameExit);
}
