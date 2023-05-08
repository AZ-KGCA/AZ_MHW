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
/** 플레이어 액션(인풋) 상태 <-플레이어 컨트롤러가 갱신
 * 이외에도 추가할 것,
 * 아이템목록불러오기 메뉴 키도 추가하되, 서버에서는 처리안하기.
 */
USTRUCT(BlueprintType)
struct FAZPlayerActionState
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector input_direction;//캐릭터의 입력방향(플레이어의 시선방향(월드방향)기준 + 입력벡터)
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_move_forward:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_move_left:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_move_back:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_move_right:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_normal_action:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_special_action:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_unique_action:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_dash_action:1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_interaction:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_evade_action:1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_use_item:1;
	
	FAZPlayerActionState()
	{
		//bitfield = Initializer 선언초기화 문법은 C++20에서가능
		//비트필드는 생성자에서 초기화해야한다.
		input_direction = FVector::Zero();//플레이어가 인지하는 입력방향
		
		bit_move_forward = false;//W
		bit_move_left = false;//A
		bit_move_back = false;//S
		bit_move_right = false;//D

		bit_normal_action = false;//MLB
		bit_special_action = false;//MRB

		bit_unique_action = false;//Ctrl

		bit_interaction = false;//F
		bit_dash_action = false;//Shift
		bit_evade_action = false;//Space
		bit_use_item = false;//E
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
	FRotator character_direction;//현재 캐릭터의 시야방향
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 max_health_point;//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 current_health_point;//체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 max_stamina;//스태미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 current_stamina;//스태미너
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 max_sharpness;//예리도//무기에 따른 변화
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 current_sharpness;//예리도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 speed_rate;//스피드 10000배 (Cast<float>)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 critical_rate;//회심율? 치명타인가 10000배 (Cast<float>)
	//무기속성
	//방어도?
	//속성별저항력?
	//회피도

	//장비벞
	//세트벞
	
	//전투
	//벞//디벞목록?(Enum?)

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_hit:1;//피격상태
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_ground:1;//지면상태인지(얼마나 떨어졋는지?)
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bAirborne:1;//절벽인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_slide:1;//미끄러지는 중인지

	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bClimb:1;//등반액션인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bSwing:1;//스윙액션인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bSwim:1;//수영액션인지
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//uint32 bMount:1;//마운트상태
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_common:1;//일반상태인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_crouch:1;//일반상태의 웅크리기
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_evade:1;//회피중인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_guard:1;//회피중인지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bit_action:1;//액션상태
	
	FAZPlayerCharacterState()
	{
		character_direction = FRotator::ZeroRotator;

		max_health_point = 100;
		current_health_point = 100;
		max_stamina = 100;
		current_stamina = 100;
		max_sharpness = 100;
		current_sharpness = 50;

		critical_rate = 0;//10000배
		speed_rate = 15000;//10000배

		bit_ground = true;
		bit_hit = false;
		bit_slide = false;

		bit_common = true;
		bit_crouch = false;
		bit_evade = false;
		bit_action = false;
		bit_guard = false;
	}
};
/** 플레이어 캐릭터의 장비 상태<-서버가 갱신
 * 
 */
USTRUCT(BlueprintType)
struct FAZPlayerEquipmentState
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 gender_type;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite)
	//int32 FaceItemID;//표정을?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 hair_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 head_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 body_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 waist_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 arm_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 leg_item_id;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 weapon_type;//비트마스크, 몽타주 등이 달라진다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 first_weapon_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 second_weapon_item_id;
	
	//슬롯에 낀 소비아이템품목
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 consume_item_id;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ammo_item_id;
	
	FAZPlayerEquipmentState()
	{
		//FaceItemID = 0;;
		gender_type = -1;//-1 F, 1 M

		//테스트 장비
		const int32 test_set_id = 297;
		body_item_id = 10000 + test_set_id;
		leg_item_id = 10500 + test_set_id;
		arm_item_id = 11000 + test_set_id;
		waist_item_id = 11500 + test_set_id;
		head_item_id = 12000 + test_set_id;
		hair_item_id = 12501 + 31;
		
		/*
		TODO: 릴리즈시 각주풀기 
		body_item_id_ = 10000+500;//Default
		leg_item_id_ = 10500+500;//Default
		arm_item_id_ = 11000+500;//Default
		waist_item_id_ = 11500+500;//Default
		head_item_id_ = 12000+500;//Default
		hair_item_id_ = 12501+0;//Default
		*/
		
		weapon_type = 0;//0~13 무기종류(14종)
		first_weapon_item_id = 1000+(weapon_type*500) + 001;//대검
		second_weapon_item_id = 0;//무기아이디
		
		consume_item_id = 101;
		ammo_item_id = 201;
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
	int32 guid_;
	
	//플레이어 입력상태(애니메이션)
	//원격PC는 입력상태값을 받아서 움직인다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAZPlayerActionState action_state_;
	
	//이 구조체는 위젯 정보용이다.(공식의 처리(실제 체크, 가감) 서버에서 처리된다.)
	//캐릭터 상태(HP, 스태미나, 예리도 등)
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAZPlayerCharacterState character_state_;
	
	//캐릭터 착용장비
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FAZPlayerEquipmentState equipment_state_;

	//캐릭터 인벤토리
	//이 구조체는 위젯 단지 인벤토리 정보출력용이다.(실제 사용은 서버에서 처리된다.)
	//예시: 물약을먹는다. 클라이언트에는 1개, 서버에는 0개있다고 가정
	//클라이언트가 이 상태에서 물약사용을 시도하면 물약섭취 애니메이션을 하긴하지만,
	//서버에서는 물약이 0개라 처리가 이루어지지못함으로 체력이 회복되지않고,
	//클라이언트에서도 체력이 회복되는 이벤트를 못받앗음으로 체력이 회복되지 않는다.
	//
	//탄약등 다양한 아이템들이 모두 마찬가지,
	//로컬 클라이언트가 시도하면, 이 구조체의 갯수 조건체크로 애니메이션은 실행되지만
	//실제처리는 서버에서 검증후 결과이벤트로 처리된다.
	//UPROPERTY(EditAnywhere,BlueprintReadWrite)
	//FAZPlayerInventoryState? Inventory{ TMap<ID,갯수>? };
};
