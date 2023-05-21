
#include "AZ_MHW/SocketHolder/PacketFunction.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/HUD/AZHUD.h"


UPacketFunction::UPacketFunction()
{
}

void UPacketFunction::LoginResponse(Login_Send_Packet* packet, bool is_successed)
{
	if (is_successed == false)
	{
		//팝업 띄우기
		AZGameInstance->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("로그인에 실패하셨습니다."), EUIMsgBoxBtnType::Confirm,
			AZGameInstance->login_mgr, TEXT(""), L"", L"", L"확인");
	}
	else
	{
		AZGameInstance->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::AuthGameServer);
	}
}

void UPacketFunction::SigninResponse(Login_Send_Packet* packet, bool is_successed)
{
	if (is_successed == false)
	{
		//팝업 띄우기
		AZGameInstance->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("회원가입에 실패하였습니다."), EUIMsgBoxBtnType::Confirm,
			AZGameInstance->login_mgr, TEXT(""), L"", L"", L"확인");
	}
	else
	{
		AZGameInstance->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("회원가입에 성공했습니다."), EUIMsgBoxBtnType::Confirm,
			AZGameInstance->login_mgr, TEXT(""), L"", L"", L"확인");
	}
}
