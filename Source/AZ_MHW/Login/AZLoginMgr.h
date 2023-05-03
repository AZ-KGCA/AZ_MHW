// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
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
		Splash,						// ���� �ΰ�.
		WaitingForAnyKey,			// �ƹ�Ű�� �����ּ���.
		LoginPageStart,				// LoginPage ������.
		ConnectLoginServerReady,	// ����� �Է�(ID, Password, Touch)�� ��ٸ���.
		ConnectLoginServer,			// �α��� ���� ����.
		AuthLoginServer,			// �α��� ���� ����.
		AuthGameServer,				// ���� ���� ���� �� ����.
		PlayerSelectEnter,			// ĳ���� ����â.(������ �÷��̾� ����Ʈ ��û)
		PlayerSelect,				// �÷��̾� ����.(����, ������ �� �� ����)
		GameMapLoading,				// ���Ӹ� �ε�.
		GameMapEnter,				// ���Ӹ� ���� �Ϸ�


		None
	};

private:
	ESequence sequence_;
	ESequence login_page_start_sequence_;

	// ���� ���� ����
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
};
