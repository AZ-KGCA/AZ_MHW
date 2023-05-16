// Copyright Team AZ. All Rights Reserved.


#include "AZPlayer_Playable.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/PlayerController/AZPlayerController_InGame.h"
#include "AZ_MHW/PlayerState/AZPlayerState.h"

#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include "AZ_MHW/Manager/AZInventoryManager.h"
#include "AZ_MHW/Item/AZItemData.h"


AAZPlayer_Playable::AAZPlayer_Playable()
{
	PrimaryActorTick.bStartWithTickEnabled = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	spring_arm_comp_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	temp_camera_comp_ = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	spring_arm_comp_->SetupAttachment(GetCapsuleComponent());
	temp_camera_comp_->SetupAttachment(spring_arm_comp_);

	spring_arm_comp_->TargetArmLength = 400.f;
	spring_arm_comp_->bUsePawnControlRotation = true;
	temp_camera_comp_->bUsePawnControlRotation = false;
}


//TODO: CameraManager 만들어서 플레이어 캐릭터와 입력처리 완전히 분리
void AAZPlayer_Playable::SetupPlayerInputComponent(UInputComponent* player_input_component)
{
	Super::SetupPlayerInputComponent(player_input_component);
	if (UEnhancedInputComponent* enhanced_input_component = CastChecked<
		UEnhancedInputComponent>(player_input_component))
	{
		enhanced_input_component->BindAction(AZGameInstance->input_mgr_->GetInputAction("Look"),
		                                     ETriggerEvent::Triggered, this, &AAZPlayer_Playable::ActionLook);
		enhanced_input_component->BindAction(AZGameInstance->input_mgr_->GetInputAction("Zoom"),
		                                     ETriggerEvent::Triggered, this, &AAZPlayer_Playable::ActionZoom);
		//...
	}
}

void AAZPlayer_Playable::PossessedBy(AController* new_controller)
{
	Super::PossessedBy(new_controller);

	playable_player_state_ = Cast<AAZPlayerState>(GetPlayerState());
	playable_player_controller_ = Cast<AAZPlayerController_InGame>(new_controller);
}

void AAZPlayer_Playable::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	//유니티처럼 콜리전용 게임오브젝트를 하나 더만들어서 액터에 붙히고, 통신하게끔하는게 
	////시야각감지(플레이어 시선방향 45도 앞 상호작용 객체 쿼리)
	////거리감지(객체에 마다의 거리체크후 쿼리)
	//역할별 콜리전 액터를 다 만든다?

	//지면감지(체공)
	//경사면감지(볼팅, 슬라이드)
	//절벽감지(벽타기)

	//물감지

	//벽감지(벽타기)
	//덩굴감지(덩굴타기)
	//인터렉션감지(채집등)

	//캐릭터 방향
	playable_player_state_->character_state_.character_direction = GetRootComponent()->GetComponentRotation();
}


void AAZPlayer_Playable::ActionLook(const FInputActionValue& value)
{
	FVector2D look_axis_vector = value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(look_axis_vector.X);
		AddControllerPitchInput(look_axis_vector.Y);
	}
}

void AAZPlayer_Playable::ActionZoom(const FInputActionValue& value)
{
	float zoom_axis_float = value.Get<float>();
	zoom_axis_float *= 10.f;

	if (spring_arm_comp_->TargetArmLength < 100 && zoom_axis_float < 0)
	{
		return;
	}
	
	if (spring_arm_comp_->TargetArmLength > 400 && zoom_axis_float > 0)
	{
		return;
	}
	
	spring_arm_comp_->TargetArmLength += zoom_axis_float;
}

void AAZPlayer_Playable::AnimNotify_OnUseItem()
{
	//on_use_item_.Broadcast();
	//const auto& item_index;// uimanager한테서 가져오기
	const auto& buff = AZGameInstance->inventory_mgr->UsePotion(0);

	switch (buff.target)
	{
	case EItemTarget::health:
		if(auto player_state = GetPlayerState())
		{
			if(auto az_player_state = Cast<AAZPlayerState>(player_state))
			{
				int32 result_health_point = 0;
				switch (buff.calc)
				{
				case ECalculation::plus:
					result_health_point = az_player_state->character_state_.current_health_point + (int)buff.effect;
					if(az_player_state->character_state_.max_health_point < result_health_point)
					{
						az_player_state->character_state_.current_health_point = az_player_state->character_state_.max_health_point;
					}
					else
					{
						az_player_state->character_state_.current_health_point = result_health_point;
					}
					UE_LOG(AZ_TEST,Warning,TEXT("%d"),az_player_state->character_state_.current_health_point);
					break;
				case ECalculation::multi:
					break;
				}
			}
		}
		break;
	case EItemTarget::damage:
		if(auto player_state = GetPlayerState())
		{
			if(auto az_player_state = Cast<AAZPlayerState>(player_state))
			{
				switch (buff.calc)
				{
				case ECalculation::plus:
					UE_LOG(AZ_TEST,Warning,TEXT("공격력 더해짐"));
					break;
				case ECalculation::multi:
					UE_LOG(AZ_TEST,Warning,TEXT("공격력 곱해서 올라감"));
					break;
				}
			}
		}
		break;
	}
	
	//사용한 아이템을 id로 넘겨줘야한다면 oneparam쓰기
	//또는 플레이어스테이트를 캐싱해서 사용한 아이템 체크하기
}
