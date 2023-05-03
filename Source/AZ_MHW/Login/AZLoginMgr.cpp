// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Login/AZLoginMgr.h"

#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/Manager/AZGameConfig.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"

UAZLoginMgr::UAZLoginMgr()
{
	sequence_ = ESequence::Splash;
	recent_server_id_ = 0;
	game_auth_serial_ = 0;
}

void UAZLoginMgr::Init()
{
	login_page_start_sequence_ = ESequence::None;
	SetServerIpPort(ESocketHolderType::Login, AZGameInstance->game_config->GetDefaultServerIP(), AZGameInstance->game_config->GetDefaultServerPort());
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
		//FGenericPlatformMisc::Requ
	}break;
	case ESequence::Splash:
	{
	}break;
	case ESequence::LoginPageStart:
	{
	}break;
	case ESequence::ConnectLoginServerReady:
	{
	}break;
	case ESequence::ConnectLoginServer:
	{
	}break;
	case ESequence::AuthLoginServer:
	{
	}break;
	case ESequence::AuthGameServer:
	{
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

void UAZLoginMgr::SetServerIpPort(ESocketHolderType holderType, const FString serverIP, int32 serverPort)
{
}
