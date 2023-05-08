// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include "PlayerController/AZPlayerController.h"//상속
#include "AZPlayerController_InGame.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration
class UInputAction;
class UInputMappingContext;

class UAZAnimInstance_Player;
class AAZPlayerState;
class AAZPlayer_Playable;
class AAZPlayer_Remotable;
#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion

/**
 * 플레이어의 입력과 서버의 이벤트로 움직이는 플레이들을 관리
 * 입력과 플레이어캐릭터 관리
 * 
 * 이 컨트롤러는 매칭이 완료된 게임에 들어간 플레이어의 컨트롤러이다.
 * 게임모드는 이 컨트롤러에게 플레이어의 캐릭터와 멀티유저의 원격 캐릭터를 준다.
 * 
 * Input_Event->PlayerState_Edit->
 * AnimUpdate_Event->AnimCondition_Check->
 * AnimInstance_Event->Condition_Edit
 */
UCLASS()
class AZ_MHW_API AAZPlayerController_InGame : public AAZPlayerController
{
	GENERATED_BODY()
public:
	AAZPlayerController_InGame();
	
#pragma region Inherited function
protected:
	virtual void SetupInputComponent() override;

	//virtual void BeginPlay() override;

	virtual void OnPossess(APawn* Pawn) override;
	
	//virtual void Tick(float DeltaTime) override;

	//virtual void BeginDestroy() override;
	
#pragma endregion 
public:
	/** 소유 플레이어 캐릭터*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	AAZPlayer_Playable* PlayableCharacter;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	UAZAnimInstance_Player* PlayableAnimInstance;
	/** 소유 플레이어 정보*/
	UPROPERTY(EditAnywhere)
	AAZPlayerState* PlayablePlayerState;
	/** 원격 플레이어 캐릭터*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TMap<int32, AAZPlayer_Remotable*> RemotableCharacterMap;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TMap<int32, UAZAnimInstance_Player*> RemotableAnimInstanceMap;
	/** 원격으로 전송받을 값 */
	UPROPERTY(EditAnywhere)
	TMap<int32, AAZPlayerState*> RemoteblePlayerStateMap;

	/** 서버에서 호출, 원격 캐릭터생성 */
	UFUNCTION()
	void CloneRemotePlayer(int32 Guid, AAZPlayerState* OtherPlayerState);
#pragma region//Input Event function
public:
	//ToDo:CameraManager만들고 Player_Playable에서 여기로 옮기기?
	//void ActionLook(const FInputActionValue& Value);//카메라 조종
	void ActionInputDirection();
	
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
	
	void ActionDashOnce_Start();	//B		(+SheatheWeapon)//이거 게임에서 안되던데..
	void ActionDashOnce_End();		//B		(+SheatheWeapon)
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


