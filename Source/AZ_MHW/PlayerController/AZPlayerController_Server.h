// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZPlayerController.h"
#include "AZPlayerController_Server.generated.h"


class AAZPlayerState_Client;
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
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
	
#pragma region login & select function(UI Call)
public:
	//접속상태의 플레이어의 보유중인 캐릭터 인덱스 데이터
	UPROPERTY() TMap<int32, FVector4d> login_player_character_list_;
	//캐릭터 인덱스에 대한 캐릭터 데이터(온라인후에 다른 캐릭터 데이터는 제거)
	UPROPERTY() TMap<int32, AAZPlayerState_Client*> login_player_character_state_;

	/** 로그인시 DB에서 캐릭터 데이터 가져와서 채우기 */
	void OnPlayerLogin(int32 client_index, int32 player_id);
	
	/** 로그아웃시 현재 서버내의 캐릭터관련 객체 전부 처리하기 */
	void OnPlayerLogout(int32 client_index);
	/** 캐릭터 선택시 선택한 캐릭터 데이터로 온라인으로 생성후 번호 부여하기 */
	void OnCharacterSelect(int32 client_index, int32 player_id, int32 character_id);

	/** 캐릭터 데이터 클라에게 보내기 */
	void OnCharacterCreate(int32 client_index, int32 player_id);
	/** 온라인시 남은 캐릭터데이터 삭제 */
	void OnCharacterDelete(int32 client_index, int32 player_id, int32 character_id);

	/** */

	/** */

	/** */

	/** */

	/** */
	
#pragma endregion

#pragma region online function
public:
	/** int32 */
	UPROPERTY() TMap<int32, AAZPlayerState_Client*> online_player_character_state_;
	/** int32 */
	UPROPERTY() TMap<int32, AAZPlayer_Origin*> online_player_characters_;
	
#pragma region Origin Part
	/** 캐릭터 선택시 선택한 캐릭터를 DB에서 가져오기*/
	void AddPlayerState_Origin(int32 client_index);
	void RemovePlayerState_Origin(int32 client_index);
	/** 나를 추가해주세요..*/
	void AddPlayer_Origin(int32 client_index);
	/** 나를 제거해주세요. 서버는 인덱스 값을 가지고 있기 때문에 보낼게 없다.*/
	void RemovePlayer_Origin(int32 client_index);
	/** 나를 움직여주세요. 서버는 인덱스값을 가지고 있기 때문에, 현재위치와 입력값을 전달*/
	void ActionPlayer_Origin(int32 client_index, FVector cur_pos, float cur_dir,float input_dir,int32 input_data);
	/** 나의 장비를 바꿔주세요. 서버는 인덱스를 가지고 있기때문에 변경할 아이템만 보내면 된다.*/
	void EquipPlayer_Origin(int32 client_index, int32 item_id);
	/** 나의 상태를 갱신하세요. 서버는 인덱스를 가지고 전달한 것이기 떄문에 변경된 상태만 보내준다.*/
	void UpdatePlayer_Origin(int32 clinet_index);
#pragma endregion 
#pragma region Remotable Part
	/** 결과값 보내기*/
	void SendPlayerSimulationResult(int32 client_index);
#pragma endregion 
};
