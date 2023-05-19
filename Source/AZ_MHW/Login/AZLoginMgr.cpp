// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Login/AZLoginMgr.h"

#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/Manager/AZGameConfig.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Widget/Login/AZWidget_Login.h"
#include "AZ_MHW/Widget/StartMenu/AZWidget_Menu.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/Widget/AZWidget_Waiting.h"
#include "Kismet/GameplayStatics.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "ShaderPipelineCache.h"

UAZLoginMgr::UAZLoginMgr()
{
	sequence_ = ESequence::Splash;
	recent_server_id_ = 0;
	game_auth_serial_ = 0;
}

void UAZLoginMgr::Init()
{
	login_page_start_sequence_ = ESequence::None;
	SetServerIpPort(ESocketHolderType::Game, AZGameInstance->game_config->GetDefaultServerIP(), AZGameInstance->game_config->GetDefaultServerPort());
}

void UAZLoginMgr::Tick(float delta_time)
{
}

bool UAZLoginMgr::IsTickable() const
{
	return !HasAnyFlags(EObjectFlags::RF_ClassDefaultObject);
}

void UAZLoginMgr::ChangeSequence(ESequence sequence, ESequence login_sequence)
{
	UAZUtility::ShippingLog(FString::Printf(TEXT("[UAZLoginMgr::ChangeSequence] Sequence=%s"), *(UAZUtility::EnumToString<ESequence>(sequence))));

	sequence_ = sequence;

	switch (sequence)
	{
	case ESequence::GameExit:
	{
		FGenericPlatformMisc::RequestExit(false);
	}break;
	case ESequence::Splash:
	{
	}break;
	case ESequence::LoginPageStart:
	{
		FShaderPipelineCache::SetBatchMode(FShaderPipelineCache::BatchMode::Background);
		FShaderPipelineCache::ResumeBatching();
		UGameplayStatics::OpenLevel(AZGameInstance->GetWorld(), FName("/Game/AZ/Map/Map_Login"));
	}break;
	case ESequence::WaitingForTouch:
	{
		UAZWidget_Login* login_page = AZGameInstance->GetHUD()->GetUI<UAZWidget_Login>(EUIName::AZWidget_Login);
		login_page->SetLoginMode(UAZWidget_Login::ELogInMode::TouchConnect);
	}break;
	case ESequence::ConnectGameServerReady:
	{
		UAZWidget_Login* login_page = AZGameInstance->GetHUD()->GetUI<UAZWidget_Login>(EUIName::AZWidget_Login);
		login_page->SetLoginMode(UAZWidget_Login::ELogInMode::IDPassword);
	}break;
	case ESequence::ConnectGameServer:
	{
		if (AZGameInstance->GetHUD())
		{
			if (auto waiting_widget = AZGameInstance->GetHUD()->OpenUI<UAZWidget_Waiting>(EUIName::AZWidget_Waiting, true))
			{
				waiting_widget->OnForceWaiting();
			}
		}
		UAZSocketHolder* socket_holder = AZGameInstance->GetSocketHolder(ESocketHolderType::Game);
		if (socket_holder == nullptr)
		{
			UAZUtility::ShippingLog(FString::Printf(TEXT("[UAZLoginMgr socketHolder null]")));
			UAZWidget_Waiting::ClearForceWaiting();
			AZGameInstance->GetHUD()->CloseUI(EUIName::AZWidget_Waiting, true);
			AZGameInstance->GetHUD()->OpenMsgBox(EUIMsgBoxType::OvertopBasic, TEXT("111"), EUIMsgBoxBtnType::OkOrCancel,
				this, TEXT("RetryReconnectRequired"), TEXT("OnServerDisconnected"), TEXT("OnServerDisconnected"), TEXT("종료"));
			return;
		}

		FString server_ip = GetServerIp(ESocketHolderType::Game);
		int32 server_port = GetServerPort(ESocketHolderType::Game);

		UAZUtility::ShippingLog(FString::Printf(TEXT("[UAZLoginMgr::ChangeSequence] login_server connect(ip=%s, port=%d)"), *server_ip, server_port));

		socket_holder->Connect(server_ip, server_port, [&](ESocketResult socket_result)
			{
				UAZWidget_Waiting::ClearForceWaiting();
				AZGameInstance->GetHUD()->CloseUI(EUIName::AZWidget_Waiting, true);
				if (socket_result == ESocketResult::Success)
				{
					ChangeSequence(ESequence::ConnectGameServerReady);
				}
				else
				{
					//팝업 띄우기
					AZGameInstance->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("111"), EUIMsgBoxBtnType::Confirm,
						this, TEXT("RetryReconnectRequired"));
					ChangeSequence(ESequence::WaitingForTouch);
				}
			});
	}break;
	case ESequence::AuthGameServer:
	{
		// FIXME Sever
		// 패킷 보내기(인증 되었다고 생각하고 넘기기)
		AZGameInstance->GetHUD()->CloseAllUI();
		AZGameInstance->GetHUD()->OpenScene<UAZWidget_Menu>(EUIName::AZWidget_Menu);
	}break;
	case ESequence::PlayerSelectEnter:
	{
	}break;
	case ESequence::PlayerSelect:
	{
	}break;
	case ESequence::GameMapEnter:
	{
	}break;
	}
}

void UAZLoginMgr::SetServerIpPort(ESocketHolderType holder_type, const FString server_ip, int32 server_port)
{
	if (holder_type < ESocketHolderType::Max)
	{
		server_ip_[(int32)holder_type] = server_ip;
		server_port_[(int32)holder_type] = server_port;
	}
}

FString UAZLoginMgr::GetServerIp(ESocketHolderType holder_type)
{
	if (holder_type < ESocketHolderType::Max)
	{
		return server_ip_[(int32)holder_type];
	}
	return TEXT("");
}

int32 UAZLoginMgr::GetServerPort(ESocketHolderType holder_type)
{
	if (holder_type < ESocketHolderType::Max)
	{
		return server_port_[(int32)holder_type];
	}
	return 0;
}

void UAZLoginMgr::OnForceKicked(EForceKick forcekick)
{
	FString kick_str = [](EForceKick reason) -> FString
	{
		switch (reason)
		{
		case EForceKick::NormalLogout: return "서버가 종료되었습니다.";
		}
		return "서버와의 연결이 끊어졌습니다.";
	}(forcekick);
	// FIXME 병합시 확인하기
	//AZGameInstance->GetHUD()->OpenMsgBox(EUIMsgBoxType::OvertopBasic, kick_str, EUIMsgBoxBtnType::Confirm, this, "OnServerDisconnected");
}
