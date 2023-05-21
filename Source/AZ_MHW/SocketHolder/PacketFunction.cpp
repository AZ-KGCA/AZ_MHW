
#include "AZ_MHW/SocketHolder/PacketFunction.h"

#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"


UPacketFunction::UPacketFunction()
{
}

void UPacketFunction::LoginResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed)
{
	if (is_successed == false)
	{
		//팝업 띄우기
		game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("로그인에 실패하셨습니다."), EUIMsgBoxBtnType::Confirm,
			game_instance_->login_mgr, TEXT(""), L"", L"", L"확인");
	}
	else
	{
		game_instance_->login_mgr->ChangeSequence(UAZLoginMgr::ESequence::AuthGameServer);
	}
}

void UPacketFunction::SigninResponse(LOGIN_RESPONSE_PACKET* packet, bool is_successed)
{
	if (is_successed == false)
	{
		//팝업 띄우기
		game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("회원가입에 실패하였습니다."), EUIMsgBoxBtnType::Confirm,
			game_instance_->login_mgr, TEXT(""), L"", L"", L"확인");
	}
	else
	{
		game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("회원가입에 성공했습니다."), EUIMsgBoxBtnType::Confirm,
			game_instance_->login_mgr, TEXT(""), L"", L"", L"확인");
	}
}

void UPacketFunction::LoginRequest(UINT32 client_index, LOGIN_REQUEST_PACKET* packet)
{
	auto P_user_id = game_instance_->ConvertCharToSqlTCHAR(packet->user_id);
	auto P_user_pw = game_instance_->ConvertCharToSqlTCHAR(packet->user_pw);
	UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	if (game_instance_->odbc->LoginCheckSQL(P_user_id, P_user_pw))
	{

		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		LOGIN_RESPONSE_PACKET login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::LOGIN_RESPONSE_SUCCESS;
		login_res_packet.packet_length = sizeof(login_res_packet);
		//login_res_packet.clinet_id = client_index;

		game_instance_->SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessLogin_Gameinstance] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		LOGIN_RESPONSE_PACKET login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::LOGIN_RESPONSE_FAIL;
		login_res_packet.packet_length = sizeof(login_res_packet);

		game_instance_->SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
}

void UPacketFunction::SignupRequest(UINT32 client_index, LOGIN_REQUEST_PACKET* packet)
{
	dbitem record;

	auto P_user_id = game_instance_->ConvertCharToSqlTCHAR(packet->user_id); ;
	auto P_user_pw = game_instance_->ConvertCharToSqlTCHAR(packet->user_pw); ;

	UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] Id : %s / PW : %s\n"), P_user_id, P_user_pw);

	record.name = P_user_id;
	record.pass = P_user_pw;

	if (game_instance_->Fclient_connect_.IsBound() == true)	game_instance_->Fclient_connect_.Execute();

	if (game_instance_->odbc->AddSQL(record))
	{
		//odbc.Load();

		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] (If) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		LOGIN_RESPONSE_PACKET login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::SIGNIN_RESPONSE_SUCCESS;
		login_res_packet.packet_length = sizeof(login_res_packet);
		//login_res_packet.clinet_id = client_index;

		game_instance_->SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ProcessSignup_GameInstance] (Else) Id : %s / PW : %s\n"), P_user_id, P_user_pw);

		LOGIN_RESPONSE_PACKET login_res_packet;
		login_res_packet.packet_id = (int)PACKET_ID::SIGNIN_RESPONSE_FAIL;
		login_res_packet.packet_length = sizeof(login_res_packet);
		//login_res_packet.clinet_id = client_index;

		game_instance_->SendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
	}
}

void UPacketFunction::RequestChatting(UINT32 client_index, LOGIN_REQUEST_PACKET* packet)
{
	LOGIN_REQUEST_PACKET login_res_packet;
	login_res_packet.packet_id = (int)PACKET_ID::CHAT_SEND_RESPONSE_SUCCESS;
	login_res_packet.packet_length = sizeof(login_res_packet);
	//login_res_packet.clinet_id = client_index;
	strcpy_s(login_res_packet.user_id, sizeof(login_res_packet.user_id), packet->user_id);

	game_instance_->BroadCastSendPacketFunc(client_index, sizeof(login_res_packet), (char*)&login_res_packet);
}

void UPacketFunction::RequestInGame()
{
	//todo
}

void UPacketFunction::RequestPlayerMove()
{
	//todo
}
