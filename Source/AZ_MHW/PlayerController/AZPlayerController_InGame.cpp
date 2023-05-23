// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_InGame.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/PlayerState/AZPlayerState.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/Character/Player/AZPlayer_Remotable.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

//#include <Components/SkinnedMeshComponent.h>

AAZPlayerController_InGame::AAZPlayerController_InGame()
{
	//PrimaryActorTick.bStartWithTickEnabled = true;
	//InternalConstructor
}

void AAZPlayerController_InGame::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	playable_player_ = Cast<AAZPlayer_Playable>(pawn);
	playable_player_state_ = GetPlayerState<AAZPlayerState>();

	PACKET_HEADER input_packet;
	// //strcpy_s(input_packet.input_position, sizeof(FVector),FV );
	// //strcpy_s(input_packet.input_direction, sizeof(FRotator), );
	input_packet.packet_id = 401;
	input_packet.packet_length = sizeof(PACKET_HEADER);
	
	//
	game_instance_->Server_Packet_Send((char*)&input_packet, input_packet.packet_length);
}

void AAZPlayerController_InGame::Tick(float delta_time)
{
	Super::Tick(delta_time);
	
	//Input_Packet input_packet;
	
	//strcpy_s(input_packet.current_position, sizeof(FVector),FV );
	//strcpy_s(input_packet.current_direction, sizeof(FVector), FR );

	//strcpy_s(input_packet.input_direction, sizeof(FRotator), FR);
	//input_packet.input_data = GetCommandBitMask();
	//AZGameInstance->client_connect->Server_Packet_Send((char*)&input_packet, input_packet.packet_length);
}

void AAZPlayerController_InGame::InitializePlayer(FAZPlayerCharacterState character_state,
	FAZPlayerEquipmentState equipment_state)
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

int32 AAZPlayerController_InGame::GetCommandBitMask()
{
	int32 result = 0;//None

	if(playable_player_state_->action_state_.bit_move_forward) result |=(1<<0);//W 1
	if(playable_player_state_->action_state_.bit_move_left) result |=(1<<1);//A 2
	if(playable_player_state_->action_state_.bit_move_back) result |=(1<<2);//S 4
	if(playable_player_state_->action_state_.bit_move_right) result |=(1<<3);//D 8

	if(playable_player_state_->action_state_.bit_normal_action) result |=(1<<4);//MLB 16
	if(playable_player_state_->action_state_.bit_special_action) result |=(1<<5);//MRB 32
	if(playable_player_state_->action_state_.bit_evade_action) result |=(1<<6);//Space 64
	if(playable_player_state_->action_state_.bit_dash_action) result |=(1<<7);//LeftShift 128
	if(playable_player_state_->action_state_.bit_unique_action) result |=(1<<8);//LeftCtrl 256

	if(playable_player_state_->action_state_.bit_use_item) result |=(1<<9);//E 512
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<10);//V
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<11);//F
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<12);//R
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<13);//C
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<14);//M

	return result;
}

void AAZPlayerController_InGame::ChangeEquipment(int32 itemID)
{
	//State에서 현재 장착아이템 아이디가져왓
	//DB에서 아이템 정보가져와서
	//state에서 빼고
	
	//DB에서 새로장착할 아이템 정보값가져와 적용하고

	//메시 변경
	playable_player_->ChangeSocketMesh(TEXT("SourceHandle"),itemID);
	playable_player_->ChangeEquipmentMesh(itemID);

	//서버에 전송(바꿀께요)
}


void AAZPlayerController_InGame::AddRemotablePlayer(int32 guid, FAZPlayerCharacterState character_state,
                                                    FAZPlayerEquipmentState equipment_state)
{
	//GetWorld()->SpawnActor<AAZPlayer_Remotable>();
	//remotable_player_map_.Add(guid,)
	
	//GetWorld()->SpawnActor<AAZPlayerState>();
	//remotable_player_state_map_.Add(guid,)

	//Initialize position, rotation
}

void AAZPlayerController_InGame::ControlRemotablePlayer(int32 guid, FAZPlayerCharacterState character_state,
	FRotator result_direction, int32 command_bitmask)
{
	if(auto player_clone = remotable_player_map_.Find(guid))
	{
		//(*player_clone)->
	}
}

void AAZPlayerController_InGame::RemoveRemotablePlayer(int32 guid)
{
	remotable_player_state_map_.Remove(guid);
	remotable_player_map_.Remove(guid);
	
	//state액터 제거하기
	//character액터 제거하기
}


/////////////////////////////////////////////////////////////////////////////////////////////////////
//input_action
/////////////////////////////////////////////////////////////////////////////////////////////////////
void AAZPlayerController_InGame::SetupInputComponent()
{
	Super::SetupInputComponent();
	game_instance_->input_mgr_->ClearInputMappingContext();
	//AZGameInstance->input_mgr->AddInputMappingContext(TEXT("UI"));

	//Camera Rotate Action + Base Move Action
	game_instance_->input_mgr_->AddInputMappingContext(TEXT("InGame"));
	SetupWeaponInputMappingContext(GetPlayerState<AAZPlayerState>()->equipment_state_.weapon_type);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UAZInputMgr* input_mgr = game_instance_->input_mgr_;
		//W
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveForward"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveForward_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveForward"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveForward_End);
		//S
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveBack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveBack_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveBack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveBack_End);
		//A
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveLeft"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveLeft_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveLeft"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveLeft_End);
		//D
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveRight"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionMoveRight_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("MoveRight"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionMoveRight_End);
		//WASD Direction
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("InputDirection"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionInputDirection);
		
		//MLB
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("NormalAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionNormalAttack_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("NormalAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionNormalAttack_End);
		//MRB
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("SpecialAttack"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionSpecialAttack_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("SpecialAttack"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionSpecialAttack_End);

		//LCtrl
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UniqueAction"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUniqueAction_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UniqueAction"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUniqueAction_End);
		//LShift
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("DashHold"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDashHold_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("DashHold"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDashHold_End);
		
		//Space
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("Dodge"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionDodge_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("Dodge"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionDodge_End);
		//E
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UseItem"), ETriggerEvent::Ongoing, this, &AAZPlayerController_InGame::ActionUseItem_Start);
		EnhancedInputComponent->BindAction(input_mgr->GetInputAction("UseItem"), ETriggerEvent::Triggered, this, &AAZPlayerController_InGame::ActionUseItem_End);
	}
}

void AAZPlayerController_InGame::ActionInputDirection()
{
	//키입력방향
	int movement_x = (static_cast<int>(playable_player_state_->action_state_.bit_move_right) - static_cast<int>(playable_player_state_->action_state_.bit_move_left));
	int movement_y = (static_cast<int>(playable_player_state_->action_state_.bit_move_back) - static_cast<int>(playable_player_state_->action_state_.bit_move_forward));
	FVector2D movement_vector(movement_x, movement_y);

	//현재 카메라방향
	FRotator control_rotation = GetControlRotation();
	FRotator yaw_rotation(0, control_rotation.Yaw, 0);
	FVector forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X);
	FVector right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y);
	
	//카메라 방향에 대한 입력방향(플레이어가 인지하는 방향)
	FVector movement_direction = (right_direction * movement_vector.Y + forward_direction * movement_vector.X).GetSafeNormal();
	playable_player_state_->action_state_.input_direction = movement_direction;
}
void AAZPlayerController_InGame::ActionMoveForward_Start()
{
	playable_player_state_->action_state_.bit_move_forward = true;
	
}
void AAZPlayerController_InGame::ActionMoveForward_End()
{
	playable_player_state_->action_state_.bit_move_forward = false;
	
}
void AAZPlayerController_InGame::ActionMoveLeft_Start()
{
	playable_player_state_->action_state_.bit_move_left = true;
	
}
void AAZPlayerController_InGame::ActionMoveLeft_End()
{
	playable_player_state_->action_state_.bit_move_left = false;
	
}
void AAZPlayerController_InGame::ActionMoveRight_Start()
{
	playable_player_state_->action_state_.bit_move_right = true;
}
void AAZPlayerController_InGame::ActionMoveRight_End()
{
	playable_player_state_->action_state_.bit_move_right = false;
}
void AAZPlayerController_InGame::ActionMoveBack_Start()
{
	playable_player_state_->action_state_.bit_move_back = true;
}
void AAZPlayerController_InGame::ActionMoveBack_End()
{
	playable_player_state_->action_state_.bit_move_back = false;
}
void AAZPlayerController_InGame::ActionUniqueAction_Start()
{
	playable_player_state_->action_state_.bit_unique_action = true;
}
void AAZPlayerController_InGame::ActionUniqueAction_End()
{
	playable_player_state_->action_state_.bit_unique_action = false;
}
void AAZPlayerController_InGame::ActionNormalAttack_Start()
{
	playable_player_state_->action_state_.bit_normal_action = true;
}
void AAZPlayerController_InGame::ActionNormalAttack_End()
{
	playable_player_state_->action_state_.bit_normal_action = false;
}
void AAZPlayerController_InGame::ActionSpecialAttack_Start()
{
	playable_player_state_->action_state_.bit_special_action = true;
}
void AAZPlayerController_InGame::ActionSpecialAttack_End()
{
	playable_player_state_->action_state_.bit_special_action = false;
}
void AAZPlayerController_InGame::ActionDashHold_Start()
{
	playable_player_state_->action_state_.bit_dash_action = true;
}
void AAZPlayerController_InGame::ActionDashHold_End()
{
	playable_player_state_->action_state_.bit_dash_action = false;
}
void AAZPlayerController_InGame::ActionDodge_Start()
{
	playable_player_state_->action_state_.bit_evade_action = true;
}
void AAZPlayerController_InGame::ActionDodge_End()
{
	playable_player_state_->action_state_.bit_evade_action = false;
}
void AAZPlayerController_InGame::ActionUseItem_Start()
{
	playable_player_state_->action_state_.bit_use_item = true;
}
void AAZPlayerController_InGame::ActionUseItem_End()
{
	playable_player_state_->action_state_.bit_use_item = false;
}
void AAZPlayerController_InGame::ActionInteract_Start()
{
	playable_player_state_->action_state_.bit_interaction = true;
}
void AAZPlayerController_InGame::ActionInteract_End()
{
	playable_player_state_->action_state_.bit_interaction = false;
}
