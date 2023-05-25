// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_InGame.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/Character/Player/AZPlayer_Remotable.h"
#include "AZ_MHW//SocketHolder/AZSocketHolder.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
//#include <Components/SkinnedMeshComponent.h>

AAZPlayerController_InGame::AAZPlayerController_InGame()
{
	spring_arm_comp_ = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	temp_camera_comp_ = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	spring_arm_comp_->SetupAttachment(GetRootComponent());
	temp_camera_comp_->SetupAttachment(spring_arm_comp_);

	spring_arm_comp_->bUsePawnControlRotation = true;
	temp_camera_comp_->bUsePawnControlRotation = false;

	bit_move_forward=false;
	bit_move_left=false;
	bit_move_back=false;
	bit_move_right=false;

	bit_normal_action=false;
	bit_special_action=false;

	bit_unique_action=false;
	bit_dash_action=false;

	bit_interaction=false;
	bit_evade_action=false;
	bit_use_item=false;
}

void AAZPlayerController_InGame::BeginPlay()
{
	Super::BeginPlay();
	
	if(GetPlayerState<AAZPlayerState_Client>() == nullptr)
	{
		UE_LOG(AZ_TEST, Error, TEXT("PlayerController_InGame: Don't Find PlayerState"));
	}
}

void AAZPlayerController_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	playable_player_state_ = GetPlayerState<AAZPlayerState_Client>();
	
	//AZGameInstance->input_mgr->AddInputMappingContext(TEXT("UI"));
	game_instance_->input_mgr_->AddInputMappingContext(TEXT("InGame"));
	SetupWeaponInputMappingContext(playable_player_state_->equipment_state_.weapon_type);

	if (UEnhancedInputComponent* enhanced_input_component = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UAZInputMgr* input_mgr = game_instance_->input_mgr_;
		//Camera Action
		enhanced_input_component->BindAction(input_mgr->GetInputAction("Look"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionInputLook);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("Zoom"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionInputZoom);

		//W
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveForward"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveForward_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveForward"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveForward_End);
		//S
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveBack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveBack_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveBack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveBack_End);
		//A
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveLeft"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveLeft_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveLeft"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveLeft_End);
		//D
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveRight"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveRight_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("MoveRight"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveRight_End);
		
		//MLB
		enhanced_input_component->BindAction(input_mgr->GetInputAction("NormalAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionNormalAttack_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("NormalAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionNormalAttack_End);
		//MRB
		enhanced_input_component->BindAction(input_mgr->GetInputAction("SpecialAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionSpecialAttack_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("SpecialAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionSpecialAttack_End);

		//LCtrl
		enhanced_input_component->BindAction(input_mgr->GetInputAction("UniqueAction"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUniqueAction_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("UniqueAction"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUniqueAction_End);
		//LShift
		enhanced_input_component->BindAction(input_mgr->GetInputAction("DashHold"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashHold_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("DashHold"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashHold_End);
		
		//Space
		enhanced_input_component->BindAction(input_mgr->GetInputAction("Dodge"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDodge_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("Dodge"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDodge_End);
		//E
		enhanced_input_component->BindAction(input_mgr->GetInputAction("UseItem"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUseItem_Start);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("UseItem"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUseItem_End);
	}
}

void AAZPlayerController_InGame::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	
	//TODO: 이미 인게임에 들어가기전에 접속을 했을때 서버에 보냇어야 함.
	//UI에서 처리하도록 변경하기
	CREATE_PLAYER_CHARACTER_PACKET packet;
	packet.packet_id = (int)PACKET_ID::CS_PLAYER_ORIGIN_CREATE_REQ;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);

	playable_player_ = Cast<AAZPlayer_Playable>(pawn);
	if(playable_player_ == nullptr)
	{
		UE_LOG(AZ_TEST,Warning,TEXT("PlayerController_InGame: Cast Failed Player Character"));
	}
	pawn->GetRootComponent()->SetWorldLocation(playable_player_state_->character_state_.character_position);
	pawn->GetRootComponent()->SetWorldRotation(playable_player_state_->character_state_.character_direction);
	
	//컴포넌트의 소유자 변경시, Rename후 outer 재설정
	spring_arm_comp_->Rename(0, pawn);
	spring_arm_comp_->AttachToComponent(pawn->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	temp_camera_comp_->Rename(0, pawn);
	temp_camera_comp_->AttachToComponent(spring_arm_comp_, FAttachmentTransformRules::KeepRelativeTransform);
	spring_arm_comp_->TargetArmLength = 400.f;
	
}

void AAZPlayerController_InGame::OnUnPossess()
{
	spring_arm_comp_->Rename(0,this);
	spring_arm_comp_->AttachToComponent(this->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
	temp_camera_comp_->Rename(0,this);
	temp_camera_comp_->AttachToComponent(spring_arm_comp_, FAttachmentTransformRules::KeepRelativeTransform);
	Super::OnUnPossess();
}

//애니메이션에서 결과를 패킷을 보내는게...
void AAZPlayerController_InGame::Tick(float delta_time)
{
	Super::Tick(delta_time);

	int32 input_bitmask = GetInputBitMask();
	float input_angle = GetInputAngle();
	
	playable_player_state_->action_state_.input_direction.Yaw = input_angle;
	playable_player_state_->action_state_.input_bitmask = input_bitmask;

	//틱마다 보낼시 1초에 약 50~60개의 패킷(더 느려질 수 있음)
	if(playable_player_ &&
		(input_bitmask & 15) &&
		(playable_player_state_->character_state_.bit_action == false))
	{
		ACTION_PLAYER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::CS_PLAYER_ORIGIN_ACTION_REQ;
		
		packet.current_position = playable_player_state_->character_state_.character_position;
		packet.current_direction = playable_player_state_->character_state_.character_direction.Yaw;
		packet.input_direction = input_angle;
		packet.input_data = input_bitmask;
		
		game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
	}
}

void AAZPlayerController_InGame::BeginDestroy()
{
	Super::BeginDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float AAZPlayerController_InGame::GetInputAngle()
{
	//현재 입력방향
	int movement_x = (static_cast<int>(bit_move_right) - static_cast<int>(bit_move_left));//입력 좌우
	int movement_y = (static_cast<int>(bit_move_forward) - static_cast<int>(bit_move_back));//입력 전후
	FVector2D movement_vector(movement_x, movement_y);
	
	//현재 카메라방향
	FRotator yaw_rotation(0, GetControlRotation().Yaw, 0);
	//UE_LOG(AZ_TEST,Warning,TEXT("%f"), yaw_rotation.Yaw);
	FVector forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X);//월드의 전후벡터
	FVector right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y);//월드의 좌우벡터
	
	//카메라 방향에 대한 입력방향(플레이어가 인지하는 방향)
	FVector movement_direction = (right_direction * movement_vector.X + forward_direction * movement_vector.Y).GetSafeNormal();
	return movement_direction.ToOrientationRotator().Yaw;
}

int32 AAZPlayerController_InGame::GetInputBitMask()
{
	int32 result = 0;//None
	
	if(bit_move_forward) result |=(1<<0);//W 1
	if(bit_move_left) result |=(1<<1);//A 2
	if(bit_move_back) result |=(1<<2);//S 4
	if(bit_move_right) result |=(1<<3);//D 8

	if(bit_normal_action) result |=(1<<4);//MLB 16
	if(bit_special_action) result |=(1<<5);//MRB 32
	if(bit_evade_action) result |=(1<<6);//Space 64
	if(bit_dash_action) result |=(1<<7);//LeftShift 128
	if(bit_unique_action) result |=(1<<8);//LeftCtrl 256

	if(bit_use_item) result |=(1<<9);//E 512
	//if(bit_use_item_) result |=(1<<10);//V
	//if(bit_use_item_) result |=(1<<11);//F
	//if(bit_use_item_) result |=(1<<12);//R
	//if(bit_use_item_) result |=(1<<13);//C
	//if(bit_use_item_) result |=(1<<14);//M
	
	return result;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_InGame::ChangeEquipment(int32 itemID)
{
}

void AAZPlayerController_InGame::BuyItem(int32 item_id, int32 item_count)
{
}

void AAZPlayerController_InGame::SellItem(int32 item_id, int32 item_count)
{
}

void AAZPlayerController_InGame::GetItem(int32 item_id, int32 item_count)
{
}

void AAZPlayerController_InGame::UseItem(int32 item_id, int32 item_count)
{
}


void AAZPlayerController_InGame::InitializePlayer(FAZPlayerCharacterState character_state, FAZPlayerEquipmentState equipment_state)
{
	
}

void AAZPlayerController_InGame::SetupWeaponInputMappingContext(int32 weapon_type)
{
	if(weapon_type > 10)//원거리 무기인 경우
	{
		game_instance_->input_mgr_->RemoveInputMappingContext(TEXT("MeleeWeapons"));
		game_instance_->input_mgr_->AddInputMappingContext(TEXT("RangedWeapons"));
	}
	else
	{
		game_instance_->input_mgr_->RemoveInputMappingContext(TEXT("RangedWeapons"));
		game_instance_->input_mgr_->AddInputMappingContext(TEXT("MeleeWeapons"));
	}
}

void AAZPlayerController_InGame::ForceInterpolation(FVector position, FRotator direction)
{
	playable_player_->SetActorLocation(position);
	playable_player_->SetActorRotation(direction);
}




void AAZPlayerController_InGame::Remotable_AddPlayer(int32 guid, const FAZPlayerCharacterState& character_state, const FAZPlayerEquipmentState& equipment_state)
{
	//GetWorld()->SpawnActor<AAZPlayer_Remotable>();
	//remotable_player_map_.Add(guid,)
	
	//GetWorld()->SpawnActor<AAZPlayerState>();
	//remotable_player_state_map_.Add(guid,)

	//Initialize position, rotation
}

void AAZPlayerController_InGame::Remotable_RemovePlayer(int32 guid)
{
	remotable_player_state_map_.Remove(guid);
	remotable_player_map_.Remove(guid);
	
	//state액터 제거하기
	//character액터 제거하기
}

void AAZPlayerController_InGame::Remotable_ControlPlayer(int32 guid, const FAZPlayerCharacterState& character_state, const FRotator& result_direction, int32 result_command_bitmask)
{
	if(auto player_clone = remotable_player_map_.Find(guid))
	{
		//(*player_clone)->
	}
}

void AAZPlayerController_InGame::Remotable_ForceUpdatePlayer(int32 guid, FVector position, FRotator direction)
{
	
}

void AAZPlayerController_InGame::Remotable_UpdatePlayerState(int32 guid, FAZPlayerCharacterState character_state)
{
	if(auto remotable_player = remotable_player_map_.Find(guid))
	{
		
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//input_action
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AAZPlayerController_InGame::ActionInputLook(const FInputActionValue& value)
{
	const FVector2D look_axis_vector = value.Get<FVector2D>();
	if (this != nullptr)
	{
		AddYawInput(look_axis_vector.X);
		AddPitchInput(look_axis_vector.Y);
	}
}
void AAZPlayerController_InGame::ActionInputZoom(const FInputActionValue& value)
{
	float zoom_axis_float = value.Get<float>();
	zoom_axis_float *= 10.f;
	if(spring_arm_comp_)
	{
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
}

void AAZPlayerController_InGame::ActionMoveForward_Start()
{
	bit_move_forward = true;
	
}
void AAZPlayerController_InGame::ActionMoveForward_End()
{
	bit_move_forward = false;
	
}
void AAZPlayerController_InGame::ActionMoveLeft_Start()
{
	bit_move_left = true;
	
}
void AAZPlayerController_InGame::ActionMoveLeft_End()
{
	bit_move_left = false;
	
}
void AAZPlayerController_InGame::ActionMoveRight_Start()
{
	bit_move_right = true;
}
void AAZPlayerController_InGame::ActionMoveRight_End()
{
	bit_move_right = false;
}
void AAZPlayerController_InGame::ActionMoveBack_Start()
{
	bit_move_back = true;
}
void AAZPlayerController_InGame::ActionMoveBack_End()
{
	bit_move_back = false;
}
void AAZPlayerController_InGame::ActionUniqueAction_Start()
{
	bit_unique_action = true;
}
void AAZPlayerController_InGame::ActionUniqueAction_End()
{
	bit_unique_action = false;
}
void AAZPlayerController_InGame::ActionNormalAttack_Start()
{
	bit_normal_action = true;
}
void AAZPlayerController_InGame::ActionNormalAttack_End()
{
	bit_normal_action = false;
}
void AAZPlayerController_InGame::ActionSpecialAttack_Start()
{
	bit_special_action = true;
}
void AAZPlayerController_InGame::ActionSpecialAttack_End()
{
	bit_special_action = false;
}
void AAZPlayerController_InGame::ActionDashHold_Start()
{
	bit_dash_action = true;
}
void AAZPlayerController_InGame::ActionDashHold_End()
{
	bit_dash_action = false;
}
void AAZPlayerController_InGame::ActionDodge_Start()
{
	bit_evade_action = true;
}
void AAZPlayerController_InGame::ActionDodge_End()
{
	bit_evade_action = false;
}
void AAZPlayerController_InGame::ActionUseItem_Start()
{
	bit_use_item = true;
}
void AAZPlayerController_InGame::ActionUseItem_End()
{
	bit_use_item = false;
}
void AAZPlayerController_InGame::ActionInteract_Start()
{
	bit_interaction = true;
}
void AAZPlayerController_InGame::ActionInteract_End()
{
	bit_interaction = false;
}
