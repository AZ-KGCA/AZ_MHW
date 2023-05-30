// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_InGame.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/Character/Player/AZPlayer_Remotable.h"
#include "AZ_MHW//SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/Util/AZUtility.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>

#include "GameMode/AZGameMode_InGame.h"
#include "HUD/AZHUD.h"
#include "HUD/AZHUDDefine.h"
#include "Widget/MsgBox/AZWidget_MsgBoxBasic.h"
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

}

//PlayerState Cache
//TempAddPlayer_Origin();
void AAZPlayerController_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	playable_player_state_ = GetPlayerState<AAZPlayerState_Client>();
	if(playable_player_state_  == nullptr) UE_LOG(AZ_PLAYER, Warning, TEXT("PlayerController_InGame: Cast Failed Player State"));
	
	//TODO 임시 캐릭터 샐럭트 창에서 호출햇어야함.
	TempSendAddPlayer_Origin();
	
	game_instance_->input_mgr_->AddInputMappingContext(TEXT("InGame"));
	SetupWeaponInputMappingContext(playable_player_state_->equipment_state_.weapon_type);

	if (UEnhancedInputComponent* enhanced_input_component = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UAZInputMgr* input_mgr = game_instance_->input_mgr_;
		//Mouse
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

		//Q
		enhanced_input_component->BindAction(input_mgr->GetInputAction("QuestTemp"), ETriggerEvent::Started, this, &AAZPlayerController_InGame::OpenQuestTemp);
		enhanced_input_component->BindAction(input_mgr->GetInputAction("TestFunction"),ETriggerEvent::Triggered,this,&AAZPlayerController_InGame::ActionTestFunction);
	}
}

//Player Cache
void AAZPlayerController_InGame::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	
	playable_player_ = Cast<AAZPlayer_Playable>(pawn);
	if(playable_player_ == nullptr)
	{
		UE_LOG(AZ_PLAYER, Warning, TEXT("PlayerController_InGame: Cast Failed Player Character"));
		return;
	}
	playable_player_->player_character_state_ = playable_player_state_;
	
	//빙의시 초기화시작
	pawn->GetRootComponent()->SetWorldLocation(playable_player_state_->character_state_.character_position);
	pawn->GetRootComponent()->SetWorldRotation(playable_player_state_->character_state_.character_direction);
	
	//폰에게 카메라 할당
	SetupFollowCameraOwnPawn(true);
	//타이머 인풋패킷
	GetWorld()->GetTimerManager().SetTimer(packet_timer_handle_, this, &AAZPlayerController_InGame::TimerUpdateInputPacket, 1/10.f, true);
}

void AAZPlayerController_InGame::OnUnPossess()
{
	GetWorld()->GetTimerManager().ClearTimer(packet_timer_handle_);
	SetupFollowCameraOwnPawn(false);
	playable_player_ = nullptr;
	Super::OnUnPossess();
}

void AAZPlayerController_InGame::Tick(float delta_time)
{
	Super::Tick(delta_time);
	
	final_input_bitmask = GetInputBitMask();
	//WASD값일시에만
	if(final_input_bitmask & 15)
	{
		final_input_angle = GetInputAngle();
	}
	
	playable_player_state_->action_state_.input_bitmask = final_input_bitmask;
	playable_player_state_->action_state_.input_direction.Yaw = final_input_angle;
}

void AAZPlayerController_InGame::BeginDestroy()
{
	Super::BeginDestroy();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_InGame::TempSendAddPlayer_Origin()
{
	//TODO 이미 인게임에 들어가기전에 접속을 했을때 서버에 보냇어야 함.
	//UI에서 처리하도록 변경하기
	//CS_PLAYER_CHARACTER_SELECT_REQ->Server
	CREATE_PLAYER_CHARACTER_PACKET packet;
	packet.packet_id = (int)PACKET_ID::CS_PLAYER_ORIGIN_CREATE_REQ;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);

	//Server->SC_PLAYER_CHARACTER_SELECT_RES

	//SC_PLAYER_PLAYABLE_CHARACTER_CREATE_RES
}

void AAZPlayerController_InGame::TempSendForceUpdatePlayer_Origin()
{
	ACTION_PLAYER_PACKET packet;
	packet.packet_id = (int)PACKET_ID::CS_DEVELOP_PLAYER_FORCE_UPDATE_CMD;
	packet.current_position = playable_player_->GetRootComponent()->GetComponentLocation();
	
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet,packet.packet_length);
}

void AAZPlayerController_InGame::AddPlayer_Playable()
{
	if(playable_player_ == nullptr)
	{
		UE_LOG(AZ_PLAYER,Warning,TEXT("Server에서 로컬 플레이어 생성 명령"));
		auto player_character = GetWorld()->SpawnActor<AAZPlayer_Playable>();
		playable_player_ = player_character;
		Possess(player_character);
	}
}

void AAZPlayerController_InGame::RemovePlayer_Playable()
{
	if(playable_player_ != nullptr)
	{
		UE_LOG(AZ_PLAYER,Warning,TEXT("Server에서 로컬 플레이어 제거 명령"));
		playable_player_->Destroy();
		playable_player_= nullptr;
		playable_player_state_->Destroy();
		playable_player_state_ = nullptr;
	}
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

void AAZPlayerController_InGame::ChangeEquipment(int32 item_id)
{
	playable_player_->ChangeEquipment(item_id);

	//Packet
}

void AAZPlayerController_InGame::BuyItem(int32 item_id, int32 item_count)
{
	//game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket();
}

void AAZPlayerController_InGame::SellItem(int32 item_id, int32 item_count)
{
	
}

void AAZPlayerController_InGame::GetItem(int32 item_id, int32 item_count)
{
}

void AAZPlayerController_InGame::UseItem(int32 item_id, int32 item_count)
{
	//playable_player_->
	//Packet
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

void AAZPlayerController_InGame::SetupFollowCameraOwnPawn(bool on_off)
{
	if(on_off)
	{
		if(const auto pawn = GetPawn())
		{
			spring_arm_comp_->Rename(nullptr, pawn);
			spring_arm_comp_->AttachToComponent(pawn->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
			temp_camera_comp_->Rename(nullptr, pawn);
			temp_camera_comp_->AttachToComponent(spring_arm_comp_, FAttachmentTransformRules::KeepRelativeTransform);
			spring_arm_comp_->TargetArmLength = 400.f;
		}
	}
	else
	{
		spring_arm_comp_->Rename(nullptr,this);
		spring_arm_comp_->AttachToComponent(this->GetRootComponent(),FAttachmentTransformRules::KeepRelativeTransform);
		temp_camera_comp_->Rename(nullptr,this);
		temp_camera_comp_->AttachToComponent(spring_arm_comp_, FAttachmentTransformRules::KeepRelativeTransform);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_InGame::ForceInterpolationPlayer_Playable(FVector position)
{
	playable_player_->SetActorLocation(position);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_InGame::AddPlayerState_Remotable(int32 guid, const FAZPlayerCharacterState& character_state, const FAZPlayerEquipmentState& equipment_state)
{
	const auto remotable_player_state = GetWorld()->SpawnActor<AAZPlayerState_Client>();
	remotable_player_state_map_.Add(guid, remotable_player_state);
	remotable_player_state->character_state_ = character_state;
	remotable_player_state->equipment_state_ = equipment_state;
}

void AAZPlayerController_InGame::AddPlayer_Remotable(int32 guid)
{
	const auto& remotable_player = GetWorld()->SpawnActor<AAZPlayer_Remotable>();
	remotable_player_map_.Add(guid, remotable_player);
	
	if(const auto& found_player_state = remotable_player_state_map_.Find(guid))
	{
		const auto& remotable_player_state = (*found_player_state);
		remotable_player->SetPlayerState(remotable_player_state);
		remotable_player->GetRootComponent()->SetWorldLocation(remotable_player_state->character_state_.character_position);
		remotable_player->GetRootComponent()->SetWorldRotation(remotable_player_state->character_state_.character_direction);
	}
}

void AAZPlayerController_InGame::RemovePlayer_Remotable(int32 guid)
{
	if(const auto& found_player_state = remotable_player_state_map_.Find(guid))
	{
		(*found_player_state)->Destroy();
		remotable_player_state_map_.Remove(guid);
	}
	if(const auto& found_player = remotable_player_map_.Find(guid))
	{
		(*found_player)->Destroy();
		remotable_player_map_.Remove(guid);
	}
}

void AAZPlayerController_InGame::ActionPlayer_Remotable(int32 guid, FVector cur_pos, float cur_dir, float input_dir, int32 input_data)
{
	if(const auto& found_player = remotable_player_map_.Find(guid))
	{
		//원본 객체에 있는 위치, 회전값 강제보간
		(*found_player)->GetRootComponent()->SetWorldLocation(cur_pos);
		(*found_player)->GetRootComponent()->SetWorldRotation(FRotator(0,cur_dir, 0));

		if((*found_player)->player_character_state_ != nullptr)
		{
			(*found_player)->player_character_state_->action_state_.input_direction.Yaw = input_dir;
			(*found_player)->player_character_state_->action_state_.input_bitmask = input_data;
		}
	}
}

void AAZPlayerController_InGame::EquipPlayer_Remotable(int32 guid, int32 item_id)
{
	if(const auto& found_player = remotable_player_map_.Find(guid))
	{
		const auto& remotable_player = (*found_player);

		//상태처리와 캐릭터메시 처리를 분리하고 싶지만 시간이 없다...
		remotable_player->ChangeEquipment(item_id);
	}
}

void AAZPlayerController_InGame::UpdatePlayerState_Remotable(int32 guid, int32 state_type, int32 state_value, int32 anim_bitmask)
{
	if(const auto& found_player = remotable_player_map_.Find(guid))
	{
		const auto& remotable_player= (*found_player);

		//캐릭터 상태 전환하기
		//remotable_player->player_character_state_->character_state_ = character_state;
	}
}

void AAZPlayerController_InGame::UpdatePlayerState_Playable(int32 state_type, int32 state_value, int32 anim_bitmask)
{
	if(playable_player_)
	{
		//캐릭터 상태 전환하기
		//playable_player_->player_character_state_->character_state_ = character_state;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//temp test_function
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AAZPlayerController_InGame::OpenQuestTemp()
{
	if (quest_msgbox_) return;
	auto msgbox = game_instance_->GetHUD()->OpenMsgBox(EUIMsgBoxType::Basic, TEXT("그리고 수아는 KGCA에"), EUIMsgBoxBtnType::Confirm,
		this, TEXT(""), L"", L"", L"당신도");

	if (msgbox)
	{
		quest_msgbox_ = Cast<UAZWidget_MsgBoxBasic>(msgbox);
		if (quest_msgbox_)
		{
			quest_msgbox_->SetTitle(TEXT("불의 용은 숲의 꼭대기에"));
			quest_msgbox_->AddHandler(EMsgEventButtonType::Left, Cast<AAZGameMode_InGame>(game_instance_->GetGameMode()), FName(TEXT("RequestWarpCombatLevel")));
		}
	}
}
void AAZPlayerController_InGame::ActionTestFunction()
{
	
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
//input_action
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AAZPlayerController_InGame::TimerUpdateInputPacket()
{
	if(is_event_input_mode_) return;
	EventUpdateInputPacket();
}
void AAZPlayerController_InGame::EventUpdateInputPacket()
{
	if((playable_player_state_ == nullptr) || (playable_player_ == nullptr)) return;

	final_input_bitmask = GetInputBitMask();
	//WASD값일시에만
	if(final_input_bitmask & 15)
	{
		final_input_angle = GetInputAngle();
	}
	
	ACTION_PLAYER_PACKET packet;
	packet.packet_id = (int)PACKET_ID::CS_PLAYER_ORIGIN_ACTION_REQ;

	packet.current_position = playable_player_->GetRootComponent()->GetComponentLocation();
	packet.current_direction = playable_player_->GetRootComponent()->GetComponentRotation().Yaw;
	packet.input_direction = final_input_angle;
	packet.input_data = final_input_bitmask;
	
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet,packet.packet_length);
}

void AAZPlayerController_InGame::ActionInputLook(const FInputActionValue& value)
{
	const FVector2D look_axis_vector = value.Get<FVector2D>();

	AddYawInput(look_axis_vector.X);
	AddPitchInput(look_axis_vector.Y);
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
	is_event_input_mode_ = false;
}
void AAZPlayerController_InGame::ActionMoveForward_End()
{
	bit_move_forward = false;
	if((GetInputBitMask() & 15) == false)
	{
		is_event_input_mode_ = true;
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionMoveLeft_Start()
{
	bit_move_left = true;
	is_event_input_mode_ = false;
}
void AAZPlayerController_InGame::ActionMoveLeft_End()
{
	bit_move_left = false;
	if((GetInputBitMask() & 15) == false)
	{
		is_event_input_mode_ = true;
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionMoveRight_Start()
{
	bit_move_right = true;
	is_event_input_mode_ = false;
}
void AAZPlayerController_InGame::ActionMoveRight_End()
{
	bit_move_right = false;
	if((GetInputBitMask() & 15) == false)
	{
		is_event_input_mode_ = true;
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionMoveBack_Start()
{
	bit_move_back = true;
	is_event_input_mode_ = false;
}
void AAZPlayerController_InGame::ActionMoveBack_End()
{
	bit_move_back = false;
	if((GetInputBitMask() & 15) == false)
	{
		is_event_input_mode_ = true;
		EventUpdateInputPacket();
	}
}

void AAZPlayerController_InGame::ActionUniqueAction_Start()
{
	bit_unique_action = true;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionUniqueAction_End()
{
	bit_unique_action = false;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionNormalAttack_Start()
{
	bit_normal_action = true;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionNormalAttack_End()
{
	bit_normal_action = false;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionSpecialAttack_Start()
{
	bit_special_action = true;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionSpecialAttack_End()
{
	bit_special_action = false;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionDashHold_Start()
{
	bit_dash_action = true;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionDashHold_End()
{
	bit_dash_action = false;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionDodge_Start()
{
	bit_evade_action = true;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionDodge_End()
{
	bit_evade_action = false;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionUseItem_Start()
{
	bit_use_item = true;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionUseItem_End()
{
	bit_use_item = false;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionInteract_Start()
{
	bit_interaction = true;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}
void AAZPlayerController_InGame::ActionInteract_End()
{
	bit_interaction = false;
	if(is_event_input_mode_)
	{
		EventUpdateInputPacket();
	}
}

