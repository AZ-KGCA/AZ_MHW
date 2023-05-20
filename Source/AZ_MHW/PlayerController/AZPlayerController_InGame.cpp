// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_InGame.h"
#include "AZ_MHW/Manager/AZInputMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/Character/Player/AZPlayer_Remotable.h"
//FIXME
#include "GameInstance/Client_To_Server.h"

#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>



//#include <Components/SkinnedMeshComponent.h>

AAZPlayerController_InGame::AAZPlayerController_InGame()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	//InternalConstructor
}

void AAZPlayerController_InGame::BeginPlay()
{
	Super::BeginPlay();
	playable_player_state_ = GetPlayerState<AAZPlayerState_Client>();
}

void AAZPlayerController_InGame::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	playable_player_ = Cast<AAZPlayer_Playable>(pawn);

	//원본 푸쉬 요청 패킷
	BasePacket request_push_origin_packet;
	request_push_origin_packet.packet_id = 401;
	request_push_origin_packet.packet_length = sizeof(BasePacket);
	
	//
	AZGameInstance->client_connect->Server_Packet_Send((char*)&request_push_origin_packet, request_push_origin_packet.packet_length);
}

void AAZPlayerController_InGame::Tick(float delta_time)
{
	Super::Tick(delta_time);
	
	InputPacket request_input_packet;
	
	//strcpy_s(input_packet.current_position, sizeof(FVector),FV );
	//strcpy_s(input_packet.current_direction, sizeof(FVector), FR );
	//strcpy_s(input_packet.input_direction, sizeof(FRotator), FR);

	request_input_packet.current_direction;
	request_input_packet.current_position;
	//request_input_packet.input_direction = playable_player_state_->action_state_.input_direction;
	request_input_packet.input_data = GetCommandBitMask();
	AZGameInstance->client_connect->Server_Packet_Send((char*)&request_input_packet, request_input_packet.packet_length);
}

void AAZPlayerController_InGame::BeginDestroy()
{
	Super::BeginDestroy();
}

int32 AAZPlayerController_InGame::GetAnimationMask()
{
	int32 result = 0;
	
	if(false) result |=(1<<0);
	if(false) result |=(1<<1);
	if(false) result |=(1<<2);
	if(false) result |=(1<<3);
	if(false) result |=(1<<4);
	if(false) result |=(1<<5);
	if(false) result |=(1<<6);
	if(false) result |=(1<<7);
	if(false) result |=(1<<8);
	if(false) result |=(1<<9);
	if(false) result |=(1<<10);
	if(false) result |=(1<<11);
	if(false) result |=(1<<12);
	if(false) result |=(1<<13);
	if(false) result |=(1<<14);
	if(false) result |=(1<<15);
	if(false) result |=(1<<16);
	if(false) result |=(1<<17);
	if(false) result |=(1<<18);
	if(false) result |=(1<<19);
	if(false) result |=(1<<20);
	if(false) result |=(1<<21);
	if(false) result |=(1<<22);
	if(false) result |=(1<<23);
	if(false) result |=(1<<24);
	if(false) result |=(1<<25);
	if(false) result |=(1<<26);
	if(false) result |=(1<<27);
	if(false) result |=(1<<28);
	if(false) result |=(1<<29);
	if(false) result |=(1<<30);
	if(false) result |=(1<<31);
	//if(false) result |=(1<<32); //int32이기 때문에... 왜 int32로 햇지?
	
	return result;
}

void AAZPlayerController_InGame::InitializePlayer(FAZPlayerCharacterState character_state,
                                                  FAZPlayerEquipmentState equipment_state)
{
	//playable_player_state_->character_state_
	
	//playable_player_state_->equipment_state_

}

void AAZPlayerController_InGame::SetupWeaponInputMappingContext(int32 weapon_type)
{
	if(weapon_type > 10)//원거리 무기인 경우
	{
		AZGameInstance->input_mgr_->RemoveInputMappingContext(TEXT("MeleeWeapons"));
		AZGameInstance->input_mgr_->AddInputMappingContext(TEXT("RangedWeapons"));
	}
	else
	{
		AZGameInstance->input_mgr_->RemoveInputMappingContext(TEXT("RangedWeapons"));
		AZGameInstance->input_mgr_->AddInputMappingContext(TEXT("MeleeWeapons"));
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

void AAZPlayerController_InGame::RequestChangeEquipment(int32 itemID)
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

void AAZPlayerController_InGame::RequestBuyItem(int32 itemID)
{
	
}


// void AAZPlayerController_InGame::Remotable_AddPlayer(int32 guid, FAZPlayerCharacterState character_state,
//                                                      FAZPlayerEquipmentState equipment_state)
// {
// 	//GetWorld()->SpawnActor<AAZPlayer_Remotable>();
// 	//remotable_player_map_.Add(guid,)
// 	
// 	//GetWorld()->SpawnActor<AAZPlayerState>();
// 	//remotable_player_state_map_.Add(guid,)
//
// 	//Initialize position, rotation
// }
//
// void AAZPlayerController_InGame::Remotable_ControlPlayer(int32 guid, FAZPlayerCharacterState character_state,
// 	FRotator result_direction, int32 command_bitmask)
// {
// 	if(auto player_clone = remotable_player_map_.Find(guid))
// 	{
// 		//(*player_clone)->
// 	}
// }

void AAZPlayerController_InGame::Remotable_RemovePlayer(int32 guid)
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
	AZGameInstance->input_mgr_->ClearInputMappingContext();
	//AZGameInstance->input_mgr->AddInputMappingContext(TEXT("UI"));

	//Camera Rotate Action + Base Move Action
	AZGameInstance->input_mgr_->AddInputMappingContext(TEXT("InGame"));
	SetupWeaponInputMappingContext(GetPlayerState<AAZPlayerState_Client>()->equipment_state_.weapon_type);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		UAZInputMgr* input_mgr = AZGameInstance->input_mgr_;
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
	const int movement_x = (static_cast<int>(playable_player_state_->action_state_.bit_move_right) - static_cast<int>(playable_player_state_->action_state_.bit_move_left));
	const int movement_y = (static_cast<int>(playable_player_state_->action_state_.bit_move_back) - static_cast<int>(playable_player_state_->action_state_.bit_move_forward));
	const FVector2D movement_vector(movement_x, movement_y);

	//현재 카메라방향
	const FRotator control_rotation = GetControlRotation();
	const FRotator yaw_rotation(0, control_rotation.Yaw, 0);
	const FVector forward_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::X);
	const FVector right_direction = FRotationMatrix(yaw_rotation).GetUnitAxis(EAxis::Y);
	
	//카메라 방향에 대한 입력방향(플레이어가 인지하는 방향)
	const FVector movement_direction = (right_direction * movement_vector.Y + forward_direction * movement_vector.X).GetSafeNormal();
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
