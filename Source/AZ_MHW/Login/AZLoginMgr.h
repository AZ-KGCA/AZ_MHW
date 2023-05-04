// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/CommonSource/Define/LoginStruct.h"
#include "Tickable.h"
#include "AZLoginMgr.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZLoginMgr : public UObject, public FTickableGameObject
{
	GENERATED_BODY()
public:
	enum class ESequence : uint8
	{
		GameExit,
		Splash,						// 시작 로고.
		WaitingForAnyKey,			// 아무키나 눌러주세요.
		LoginPageStart,				// LoginPage 시작점.
		ConnectLoginServerReady,	// 사용자 입력(ID, Password, Touch)를 기다린다.
		ConnectLoginServer,			// 로그인 서버 접속.
		AuthLoginServer,			// 로그인 서버 인증.
		AuthGameServer,				// 게임 서버 접속 및 인증.
		PlayerSelectEnter,			// 캐릭터 선택창.(생성된 플레이어 리스트 요청)
		PlayerSelect,				// 플레이어 선택.(생성, 삭제도 할 수 있음)
		GameMapLoading,				// 게임맵 로딩.
		GameMapEnter,				// 게임맵 입장 완료


		None
	};

private:
	ESequence sequence_;
	ESequence login_page_start_sequence_;

	// 서버 선택 관련
	int32 recent_server_id_;
	int32 game_auth_serial_;
	FString server_ip_[(int32)ESocketHolderType::Max];
	FString server_port_[(int32)ESocketHolderType::Max];


public:
	UAZLoginMgr();

	virtual void Tick(float delta_time) override;
	virtual bool IsTickable() const override;
	virtual TStatId GetStatId() const override { return TStatId(); }

	void Init();
	void ChangeSequence(ESequence sequnce, ESequence login_sequence = ESequence::None);
	void SetServerIpPort(ESocketHolderType holderType, const FString serverIP, int32 serverPort);

	ESequence GetSequence() { return sequence_; }
	ESequence GetLoginPageStartSequence() { return login_page_start_sequence_; }
	int32 GetGameAuthSerial() { return game_auth_serial_; }

	void OnForceKicked(EForceKick forcekick);
};
