// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <GameFramework/PlayerState.h>
#include <AZPlayerState.generated.h>

#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration

#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct
/** 플레이어 액션(인풋) 상태 <-컨트롤러가 갱신
 * 이외에도 추가할 것,
 * 아이템목록불러오기 메뉴 키도 추가하되, 서버에서는 처리안하기.
 */
USTRUCT(BlueprintType)
struct FAZPlayerActionState
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InputDirection;//캐릭터의 입력방향(플레이어의 시선방향(월드방향)기준 + 입력벡터)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bMoveForward:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bMoveBack:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bMoveLeft:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bMoveRight:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bNormalAttack:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bSpecialAttack:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bUniqueWeaponAction:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bDashHold:1;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bDashOnce:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bEvade:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bUseItem:1;
	FAZPlayerActionState()
	{
		//bitfield = Initializer 선언초기화 문법은 C++20에서가능
		//비트필드는 생성자에서 초기화해야한다.
		InputDirection = FVector::Zero();//플레이어가 인지하는 입력방향
		
		bMoveForward = false;//W
		bMoveBack = false;//S
		bMoveLeft = false;//A
		bMoveRight = false;//D

		bNormalAttack = false;//MLB
		bSpecialAttack = false;//MRB

		bUniqueWeaponAction = false;//Ctrl
		
		bDashHold = false;//Shift
		
		bEvade = false;//Space
		bUseItem = false;//E

		//bDashOnce = false;//게임에 없던데
	}
};
/** 플레이어 캐릭터의 상태<-플레이어 캐릭터가 갱신
 * 
 */
USTRUCT(BlueprintType)
struct FAZPlayerCharacterState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator CharacterDirection;//현재 캐릭터의 시야방향
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealthPoint;//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentHealthPoint;//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxStamina;//스태미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentStamina;//스태미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxSharpness;//예리도//무기에 따른 변화
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentSharpness;//예리도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ActionSpeed;//스피드 10000배 (Cast<float>)

	//방어도?
	//저항력?
	//회피도
	//디버프목록?(Enum?)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bHit:1;//피격상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bGround:1;//지면상태인지(얼마나 떨어졋는지?)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bAirborne:1;//절벽인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bSlide:1;//미끄러지는 중인지

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bClimb:1;//등반액션인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bSwing:1;//스윙액션인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bSwim:1;//수영액션인지
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bCommon:1;//일반상태인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bCrouch:1;//일반상태의 웅크리기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bEvade:1;//회피중인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bAction:1;//액션상태
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bMount:1;//마운트상태
	
	
	FAZPlayerCharacterState()
	{
		CharacterDirection = FRotator::ZeroRotator;

		MaxHealthPoint = 100;
		CurrentHealthPoint = 100;
		MaxStamina = 100;
		CurrentStamina = 100;
		MaxSharpness = 100;
		CurrentSharpness = 50;
		
		ActionSpeed = 15000;//10000배

		bHit = false;
		bGround = true;
		bSlide = false;

		bCommon = true;
		bCrouch = false;
		bEvade = false;
		bAction = false;
	}
};
/** 플레이어 캐릭터의 장비 상태
 * 
 */
USTRUCT(BlueprintType)
struct FAZPlayerEquipmentState
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 FaceItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HairItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HeadItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BodyItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WaistItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ArmItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LegItemID;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponTypeID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponItemID;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 SecondWeaponItemID;
	
	//슬롯에 낀 소비아이템품목
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConsumeItemID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoItemID;
	
	FAZPlayerEquipmentState()
	{
		FaceItemID = 0;//0~100;
		HairItemID = 121;//100~130
		HeadItemID = 500;//1~XXX, 500
		BodyItemID = 500;//501
		WaistItemID = 500;
		ArmItemID = 500;
		LegItemID = 500;
		
		WeaponTypeID = 0;//0~13 무기종류
		WeaponItemID = 0;//무기아이디
		//SecondWeaponItemID = 0;//무기아이디
		
		AmmoItemID = 0;
		ConsumeItemID = 101;
	}
};
/** 플레이어 제스쳐? 표정? 머리색? 피부색? */
//FAZPlayerDecorationState?
#pragma endregion

/**
 * 클라이언트유저 소유정의 = PlayerController + PlayerPawn + PlayerState
 * 실질적인 소유는 없다. 서버에서 전부 만들어서 전달
 * 모든 유저데이터와 현재유저의 상태(어떤 조작, 어떤 메뉴 등)인지 담아두는 객체
 * 맵을 넘겨도 사라지지 않는다.
 */
UCLASS()
class AZ_MHW_API AAZPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AAZPlayerState();

#pragma region Inherited function
protected:
	/** */
	virtual void BeginPlay() override;
	/** */
	virtual void BeginDestroy() override;
#pragma endregion
public:
	/** 이 PlayerState 고유 ID */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GUID;
	
	//현재 유저조작상태(메뉴, 로비, 캐릭터만들기, 인게임, 시퀀스 등)
	//이게 필요한가? 애초에 enum으로 만들어야됨. 일단은 작성한 것(필요하면 구현하기)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 UserControlState;
	
	//플레이어 입력상태(애니메이션)
	//원격PC는 입력상태값을 받아서 움직인다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAZPlayerActionState ActionState;
	
	//이 구조체는 위젯 정보용이다.(공식의 처리(실제 체크, 가감) 서버에서 처리된다.)
	//캐릭터 상태(HP, 스태미나, 예리도 등)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAZPlayerCharacterState CharacterState;
	
	//캐릭터 착용장비
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAZPlayerEquipmentState EquipmentState;

	//캐릭터 인벤토리
	//이 구조체는 위젯 단지 인벤토리 정보출력용이다.(실제 사용은 서버에서 처리된다.)
	//예시: 물약을먹는다. 클라이언트에는 1개, 서버에는 0개있다고 가정
	//클라이언트가 이 상태에서 물약사용을 시도하면 물약섭취 애니메이션을 하긴하지만,
	//서버에서는 물약이 0개라 처리가 이루어지지못함으로 체력이 회복되지않고,
	//클라이언트에서도 체력이 회복되는 이벤트를 못받앗음으로 체력이 회복되지 않는다.
	//
	//탄약등 다양한 아이템들이 모두 마찬가지,
	//로컬 클라이언트가 시도하면, 애니메이션은 호출되지만 실제처리는 서버에서 검증.
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//FAZPlayerInventoryState? Inventory{ TMap<ID,갯수>? };
};
