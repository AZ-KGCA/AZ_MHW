// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "AZ_MHW/PlayerController/AZPlayerController.h"//상속
#include "AZPlayerController_InGame.generated.h"

struct FInputActionValue;
struct FAZPlayerEquipmentState;
struct FAZPlayerCharacterState;

class UInputAction;
class UInputMappingContext;
class USpringArmComponent;
class UCameraComponent;

class UAZAnimInstance_Player;
class UAZAnimInstance_Playable;
class UAZAnimInstance_Remotable;

class AAZPlayerState_Client;
class AAZPlayer_Playable;
class AAZPlayer_Remotable;

/**
 * 플레이어의 입력과 서버의 이벤트로 움직이는 플레이들을 관리
 * 입력과 플레이어캐릭터 관리
 * 
 * 이 컨트롤러는 매칭이 완료된 게임에 들어간 플레이어의 컨트롤러이다.
 * 
 */
UCLASS()
class AZ_MHW_API AAZPlayerController_InGame : public AAZPlayerController
{
	GENERATED_BODY()
	
public:
	AAZPlayerController_InGame();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void SetupInputComponent() override;
	/** */
	virtual void OnPossess(APawn* pawn) override;
	/** */
	virtual void OnUnPossess() override;
	/** */
	virtual void Tick(float delta_time) override;
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
	
public:
	UFUNCTION(BlueprintCallable)
	float GetInputAngle();
	UFUNCTION(BlueprintCallable)
	int32 GetInputBitMask();

	/** 장비변경에 의한 근거리, 원거리 조작 매핑 변경 */
	void SetupWeaponInputMappingContext(int32 weapon_type);
	/** 소유 폰에 대한 팔로우 카메라 설정 */
	void SetupFollowCameraOwnPawn(bool on_off);

	
#pragma region InGame Item Control(SendPacket)
	/** UI에서 호출하거나, 버튼 이벤트에 심어주세요 */
	UFUNCTION(BlueprintCallable)
	void ChangeEquipment(int32 item_id);
	UFUNCTION(BlueprintCallable)
	void BuyItem(int32 item_id, int32 item_count = 1);
	UFUNCTION(BlueprintCallable)
	void SellItem(int32 item_id, int32 item_count = 1);
	UFUNCTION(BlueprintCallable)
	void GetItem(int32 item_id, int32 item_count = 1);
	UFUNCTION(BlueprintCallable)
	void UseItem(int32 item_id, int32 item_count =1);
#pragma endregion
	
#pragma region InGame Playable Control(Process)
	/** TODO 임시함수
	 * 원래는 캐릭터 샐럭트 창에서 호출햇어야함, 현재는 BeginPlay에서 호출한다.
	 * 원래는 캐릭터 샐럭트 창에서 캐릭터 상태를 초기화해야 한다. */
	void TempSendAddPlayer_Origin();

	
	/** 소유 플레이어 캐릭터 */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AAZPlayer_Playable* playable_player_;
	/** 소유 플레이어 정보 */
	UPROPERTY(EditAnywhere)
	AAZPlayerState_Client* playable_player_state_;
	
	/** 생성한다.*/
	void AddPlayer_Playable();
	/** 제거한다.*/
	void RemovePlayer_Playable();
	/** 강제 보간적용 */
	void FerpPlayer_Playable(FVector position, FRotator direction);
	/** 서버 상태갱신*/
	void UpdatePlayerState_Playable(const FAZPlayerCharacterState& character_state);

#pragma endregion

#pragma region InGame Remotable Control(Process)
	/** 원격 플레이어 캐릭터 맵*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TMap<int32, AAZPlayer_Remotable*> remotable_player_map_;
	/** 액터의 제거를 쉽게 하기 위해서 소유 */
	UPROPERTY(EditAnywhere)
	TMap<int32, AAZPlayerState_Client*> remotable_player_state_map_;

	void AddPlayerState_Remotable(int32 guid, const FAZPlayerCharacterState& character_state, const FAZPlayerEquipmentState& equipment_state);
	/** 서버에서 호출하여, 클라에 원격 캐릭터 생성 (접속)*/
	void AddPlayer_Remotable(int32 guid);
	/** 서버에서 호출하여, 클라에 원격 캐릭터 제거 (접속종료)*/
	void RemovePlayer_Remotable(int32 guid);
	/** 서버에서 호출하여, 클라에 원격 캐릭터 조종 (다른 클라의 인풋이벤트->서버 원본 캐릭터를 조종-> 원격 전파)*/
	void ActionPlayer_Remotable(int32 guid, FVector cur_pos, float cur_dir, float input_dir, int32 input_data);
	/** */
	void EquipPlayer_Remotable(int32 guid, int32 item_id);
	/** 서버에서 호출하여, 상태 갱신*/
	void UpdatePlayerState_Remotable(int32 guid, const FAZPlayerCharacterState& character_state);
#pragma endregion
	
#pragma region Input Event function
#pragma region Input BitMask Variable
	
	uint32 bit_move_forward:1;
	uint32 bit_move_left:1;
	uint32 bit_move_back:1;
	uint32 bit_move_right:1;

	uint32 bit_normal_action:1;
	uint32 bit_special_action:1;

	uint32 bit_unique_action:1;
	uint32 bit_dash_action:1;

	uint32 bit_interaction:1;
	uint32 bit_evade_action:1;
	uint32 bit_use_item:1;
	uint32 bit_map_action:1;

	
#pragma endregion 
public:
	UPROPERTY(VisibleInstanceOnly)
	USpringArmComponent* spring_arm_comp_;//CameraControll
	UPROPERTY(VisibleInstanceOnly)
	UCameraComponent* temp_camera_comp_;//Camera
	
	void ActionInputLook(const FInputActionValue& value);
	void ActionInputZoom(const FInputActionValue& value);
	
	/** MeleeContext */
	void ActionMoveForward_Start();	//W
	void ActionMoveForward_End();	//W
	void ActionMoveLeft_Start();	//A
	void ActionMoveLeft_End();		//A
	void ActionMoveRight_Start();	//D
	void ActionMoveRight_End();		//D
	void ActionMoveBack_Start();	//S
	void ActionMoveBack_End();		//S
	
	void ActionUniqueAction_Start();//Ctrl	(Guard or DrawWeapon)
	void ActionUniqueAction_End();	//Ctrl	(Guard or DrawWeapon)
	void ActionNormalAttack_Start();//MLB	(+DrawnWeapon)
	void ActionNormalAttack_End();	//MLB	(+DrawnWeapon)
	void ActionSpecialAttack_Start();//MRB	(+DrawnWeapon)
	void ActionSpecialAttack_End();	//MRB	(+DrawnWeapon)
	void ActionDodge_Start();		//Space (Crouch)
	void ActionDodge_End();			//Space (Crouch)
	
	void ActionDashHold_Start();	//Shift (+SheatheWeapon)
	void ActionDashHold_End();		//Shift (+SheatheWeapon)
	void ActionUseItem_Start();		//E		(+SheatheWeapon)
	void ActionUseItem_End();		//E		(+SheatheWeapon)

	void ActionInteract_Start();	//F		(Interact, Speak, Gather, Carve)
	void ActionInteract_End();		//F		(Interact, Speak, Gather, Carve)
	
#pragma region 추후구현_밀리추가+원거리전체
	/*
	//나중에 구현하기
	void ActionFocusCamera();		//F, N	(SelectTarget)
	void ActionWildlifeMap();		//M
	void DisplaySlingerAimingReticle_PO();//C (Press Once)
	void DisplaySlingerAimingReticle_H();//V (Hold)
	void UseSlinger();				//MLB	(While Reticle Is Display)

	void ActionDisplayChat();		//Insert
	void ActionHideChat();			//Insert,ESC
	void ActionOpenMenu();			//ESC
	void DisplayItems();			//Q
	void BrowseItemLeft();			//MLB	(When Items Are Displayed)
	void BrowseItemRight();			//MRB	(When Items Are Displayed)
	void BrowseCoatingUp();			//MWU	(When Items Are Displayed)
	void BrowseCoatingDown();		//MWD	(When Items Are Displayed)
	void BrowseItemLeft();			//MWU
	void BrowseItemRight();			//MWD
	void BrowseAmmo_CoatingUp();	//ArrowUp
	void BrowseAmmo_CoatingDown();	//ArrowDown
	*/
	//RangeContext
	/*
	 *다른 액션
	void FireShoot();				//MLB	(weapon Drawn) 
	void Reload();					//R		(ApllyCoating, RemoveCoating)
	void DisplayAimingReticle_PO();	//C		(Press Once)
	void DrawWeapon();				//MB4, H (StandardAttack)??
	*/
	//키가 변경된액션
	/* void BrowseItemLeft();		//LeftArrow */
	/* void BrowseItemRight();		//RightArrow */
	/* void BrowseAmmo_CoatingUp();	//MWU  */
	/* void BrowseAmmo_CoatingDown();//MHD  */
#pragma endregion 
#pragma region 클러치클로
	//클러치 클로
	//상처치 분노치
	//동글 20 25
	//세모 50 0(중량무기시 100)
	//R2 0 50
	//100이상일때 효과적용(상처,분노)
	
	//조준 MRB로 추정
	//클러치상태(세모 클로공격(상처), 동글 뼈치키(방향), R2 전탄발사(벽꿍))
	//세모 동글 둘다 상처치, 세모사용시 슬링어 떨굼(주워먹은 슬링어만 발사가능)

	//동글과 R2 모두 비분노일때 머리일때만

	//발도상태기준
	//근접 L2+동글 바로
	
	//활 R3 모드변경후 L2 동글
#pragma endregion
#pragma endregion 
};


