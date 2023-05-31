// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayerController.h"
#include "InputActionValue.h"
#include "AZPlayerController_Server.generated.h"


struct FAZPlayerActionState;
class AAZPlayerState_Client;
class AAZPlayer_Origin;

/**
 * 서버용 클래스 인게임 맵에서 클라이언트의 인풋처리를 시뮬레이션 한다.
 * 클라이언트의 입력값을 서버 인게임 시뮬레이션의 캐릭터로 전달하는 역할입니다.
 * 또한 그러한 입력의 결과를 각 클라이언트의 원격 객체에 전파하는 역할도 있습니다. 
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
	//virtual void Tick(float delta_seconds) override;
	/** */
	virtual void SetupInputComponent() override;
	/** */
	virtual void OnPossess(APawn* pawn) override;
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
public:
	void TempDevelopForceUpdatePlayer_Origin(int32 client_index, FVector pos);

//인게임 접속전의 서버 관리자가 따로 있다면 필요한 기능은 아니다.
#pragma region login & select function(UI -> DB Call)
public:
	//접속상태의 플레이어의 보유중인 캐릭터 인덱스 데이터
	UPROPERTY() TMap<int32, FVector4d> login_player_character_list_;
	//캐릭터 인덱스에 대한 캐릭터 데이터(온라인후에 다른 캐릭터 데이터는 제거)
	UPROPERTY() TMap<int32, AAZPlayerState_Client*> login_player_character_state_;

	/** 클라가 로그인시 DB에서 캐릭터 데이터 가져와서 채우기 */
	void LoginPlayer(int32 client_index, FVector4d character_list = FVector::ZeroVector);
	/** 클라가 로그아웃시 현재 서버내의 캐릭터관련 객체 전부 처리하기 */
	void LogoutPlayer(int32 client_index);
	
	/** 클라가 캐릭터 선택시 선택한 캐릭터 데이터로 온라인으로 생성후 번호 부여하기 */
	void SelectPlayerCharacter(int32 client_index,  int32 selected_character_id);

	/** 캐릭터 데이터 클라에게 보내기 */
	void CreatePlayerCharacter(int32 client_index);
	/** 온라인시 남은 캐릭터데이터 삭제 */
	void DeletePlayerCharacter(int32 client_index, int32 character_id);

	
#pragma endregion

#pragma region online function
public:
	/** int32 */
	UPROPERTY() TMap<int32, AAZPlayerState_Client*> online_player_character_state_;
	/** int32 */
	UPROPERTY() TMap<int32, AAZPlayer_Origin*> online_player_characters_;
	
#pragma region Receive Part(client->server)
	/** 캐릭터 선택시 선택한 캐릭터를 DB에서 가져오기*/
	void AddPlayerState_Origin(int32 client_index);
	/** 캐릭터 데이터 제거하기*/
	void RemovePlayerState_Origin(int32 client_index);

	/** 원본을 추가해주세요..*/
	void AddPlayer_Origin(int32 client_index);
	/** 원본을 제거해주세요. 서버는 인덱스 값을 가지고 있기 때문에 보낼게 없다.*/
	void RemovePlayer_Origin(int32 client_index);

	/** 원본을 움직여주세요. 서버는 인덱스값을 가지고 있기 때문에, 현재위치와 입력값을 전달*/
	void ActionPlayer_Origin(int32 client_index, FVector cur_pos, float input_dir, int32 input_data);
	/** 원본의 장비를 바꿔주세요. 서버는 인덱스를 가지고 있기때문에 변경할 아이템만 보내면 된다.*/
	void EquipPlayer_Origin(int32 client_index, int32 item_id);
	/** 원본의 제스쳐를 실행해 주세요.*/
	void GesturePlayer_Origin(int32 client_index, int32 gesture_id);
#pragma endregion

#pragma region Send Part(server->client)
private:
	void BroadCast_AddPlayerState_Remotable(int32 client_index);
	void BroadCast_RemovePlayerState_Remotable(int32 client_index);
	
	void BroadCast_AddPlayer_Remotable(int32 client_index);
	void BroadCast_RemovePlayer_Remotable(int32 client_index);

	void BroadCast_ActionPlayer_Remotable(int32 client_index);
	void BroadCast_EquipPlayer_Remotable(int32 client_index, int32 item_id);
	void BroadCast_GesturePlayer_Remotable(int32 client_index, int32 gesture_id);
	
	void BroadCast_HitPlayer_Remotable(int32 client_index, float angle, int32 damage);
	void Send_HitPlayer_Playable(int32 client_index, float angle, int32 damage);
	void BroadCast_UpdatePlayerState_Remotable(int32 client_index, int32 capture_type, int32 capture_value, int32 capture_mask);
	void Send_UpdatePlayerState_Playable(int32 client_index, int32 capture_type, int32 capture_value, int32 capture_mask);
#pragma endregion
#pragma endregion 
};
