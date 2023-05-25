// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayerController.h"
#include "AZPlayerController_Server.generated.h"


struct FAZPlayerActionState;
class AAZPlayer_Origin;

/**
 * 서버용 클래스
 * 클라이언트의 입력값을 서버 인게임 시뮬레이션의 캐릭터로 전달하는 역할입니다.
 */
UCLASS()
class AZ_MHW_API AAZPlayerController_Server : public AAZPlayerController
{
	GENERATED_BODY()

public:
	AAZPlayerController_Server();
#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void SetupInputComponent() override;
	/** */
	virtual void OnPossess(APawn* pawn) override;
#pragma endregion
public:
	/** int32 = guid 개념 */
	UPROPERTY() TMap<int32, AAZPlayer_Origin*> logined_player_characters_;
	
	/** 인게임 입장->클라로 초기화 + 원격생성명령*/
	void Origin_AddPlayer(int32 client_index);
	/** 인게임 퇴장->클라로 맵변경 + 원격제거 명령*/
	void Origin_RemovePlayer(int32 client_index);
	/** 입력값 받기*/
	void Origin_ControlPlayer(int32 client_index, FVector cur_pos,float cur_dir, float input_dir, int32 input_data);

	/** */
	
	
	/** 결과값 보내기*/
	void SendPlayerSimulationResult(int32 client_index );
};
