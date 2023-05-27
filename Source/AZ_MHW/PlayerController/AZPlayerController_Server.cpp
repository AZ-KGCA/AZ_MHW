// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_Server.h"

#include "Character/Player/AZPlayer_Origin.h"
#include "Character/Player/AZPlayer_Playable.h"
#include "GameInstance/AZGameInstance.h"
#include "PlayerState/AZPlayerState_Client.h"
#include "GameInstance/CommonPacket.h"


AAZPlayerController_Server::AAZPlayerController_Server()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AAZPlayerController_Server::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAZPlayerController_Server::SetupInputComponent()
{
	Super::SetupInputComponent();
	
}

void AAZPlayerController_Server::OnPossess(APawn* pawn)
{
	Super::OnPossess(pawn);
	
}

void AAZPlayerController_Server::BeginDestroy()
{
	Super::BeginDestroy();
	//서버가 꺼지면 저장
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_Server::AddPlayerState_Origin(int32 client_index)
{
	AAZPlayerState_Client* origin_player_state = nullptr;
	// if(auto const found_player_state = login_player_character_state_.Find(client_index))
	// {
	// 	origin_player_state = (*found_player_state);
	// 	//로그인한 플레어어의 선택한 캐릭터를 온라인 플레이어 map으로 넘기기
	// }

	//임시
	origin_player_state = GetWorld()->SpawnActor<AAZPlayerState_Client>();
	
	//DB에서 가져와서 채우기
	//origin_player_state->equipment_state_.
	//origin_player_state->character_state_.
	
	online_player_character_state_.Add(client_index, origin_player_state);
}

void AAZPlayerController_Server::RemovePlayerState_Origin(int32 client_index)
{
	if(const auto found_player_state_origin = online_player_character_state_.Find(client_index))
	{
		(*found_player_state_origin)->Destroy();
		online_player_character_state_.Remove(client_index);
	}
}


void AAZPlayerController_Server::AddPlayer_Origin(int32 client_index)
{
	//TODO 임시(UI에서 호출햇어야함)
	AddPlayerState_Origin(client_index);

	//원본 캐릭터 생성
	auto player_origin = GetWorld()->SpawnActor<AAZPlayer_Origin>();
	if(const auto player_state_origin = online_player_character_state_.Find(client_index))
	{
		player_origin->SetPlayerState(*player_state_origin);
		player_origin->player_character_state_ = (*player_state_origin);
		
		player_origin->GetRootComponent()->SetWorldLocation(player_origin->player_character_state_->character_state_.character_position);
		player_origin->GetRootComponent()->SetWorldRotation(player_origin->player_character_state_->character_state_.character_direction);
		
		online_player_characters_.Add(client_index, player_origin);
	}

	//원본 캐릭터 생성후 로컬 생성 호출
	{
		CREATE_PLAYER_CHARACTER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_CHARACTER_SELECT_RES;
		game_instance_->SendPacketFunc(client_index, sizeof(packet), (char*)&packet);
	}
	
	//TODO 접송중인 모든 유저에게 이번에 접속한 유저를 브로드캐스트 리모터블 플레이어로 생성(자기자신은 제외)
	for(auto online_player_pair : online_player_characters_)
	{
		const auto& online_player_index = (online_player_pair.Key);
		const auto& online_player = (online_player_pair.Value);

		if(client_index != online_player_index)
		{
			INITIALIZE_PLAYER_STATE_PACKET packets;
			packets.packet_id = (int)PACKET_ID::SC_PLAYER_STATE_REMOTABLE_CREATE_CMD;
			
			packets.head_id = online_player->player_character_state_->equipment_state_.head_item_id;
			packets.body_id = online_player->player_character_state_->equipment_state_.body_item_id;
			packets.waist_id = online_player->player_character_state_->equipment_state_.waist_item_id;
			packets.hair_id = online_player->player_character_state_->equipment_state_.hair_item_id;
			packets.arm_id = online_player->player_character_state_->equipment_state_.arm_item_id;
			packets.leg_id = online_player->player_character_state_->equipment_state_.leg_item_id;
			
			packets.weapon_id = online_player->player_character_state_->equipment_state_.weapon_type;
			packets.first_id = online_player->player_character_state_->equipment_state_.first_weapon_item_id;
			packets.second_id = online_player->player_character_state_->equipment_state_.second_weapon_item_id;
			packets.pos = online_player->player_character_state_->character_state_.character_position;
			packets.dir = online_player->player_character_state_->character_state_.character_direction.Yaw;
			packets.guid = online_player_index;
			game_instance_->SendPacketFunc(online_player_index, packets.packet_length,(char*)&packets);

			CREATE_PLAYER_CHARACTER_PACKET packet;
			packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_CREATE_CMD;
			packet.guid = online_player_index;
			game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
		}
	}
	//TODO 접속한 유저는 접속중인 모든 유저를 리모터블로 생성
	{
		INITIALIZE_PLAYER_STATE_PACKET packets;
		packets.packet_id = (int)PACKET_ID::SC_PLAYER_STATE_REMOTABLE_CREATE_CMD;
			
		CREATE_PLAYER_CHARACTER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_CREATE_CMD;
		
		//TODO 접속한 유저가 현재 접속한 모든 유저들을 브로드캐스트 플레이어로 생성
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);
			
			if(client_index != online_player_index)
			{
				packets.head_id = online_player->player_character_state_->equipment_state_.head_item_id;
				packets.body_id = online_player->player_character_state_->equipment_state_.body_item_id;
				packets.waist_id = online_player->player_character_state_->equipment_state_.waist_item_id;
				packets.hair_id = online_player->player_character_state_->equipment_state_.hair_item_id;
				packets.arm_id = online_player->player_character_state_->equipment_state_.arm_item_id;
				packets.leg_id = online_player->player_character_state_->equipment_state_.leg_item_id;
			
				packets.weapon_id = online_player->player_character_state_->equipment_state_.weapon_type;
				packets.first_id = online_player->player_character_state_->equipment_state_.first_weapon_item_id;
				packets.second_id = online_player->player_character_state_->equipment_state_.second_weapon_item_id;

				packets.pos = online_player->player_character_state_->character_state_.character_position;
				packets.dir = online_player->player_character_state_->character_state_.character_direction.Yaw;
				packets.guid = online_player_index;
				game_instance_->SendPacketFunc(client_index, packets.packet_length,(char*)&packets);

				
				packet.guid = online_player_index;
				game_instance_->SendPacketFunc(client_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}

void AAZPlayerController_Server::RemovePlayer_Origin(int32 client_index)
{
	if(const auto player_origin = online_player_characters_.Find(client_index))
	{
		online_player_characters_.Remove(client_index);
		(*player_origin)->Destroy();//?

		//서버에서 클라로 해당 id의 원격 캐릭터 제거 명령
	}
}

void AAZPlayerController_Server::ActionPlayer_Origin(int32 client_index, FVector cur_pos, float cur_dir, float input_dir, int32 input_data)
{
	//cur_pos, cur_dir이 심하게 차이나면 클라로 ForceUpdate 전송
	//위치
	//회전
	
	if(const auto player = online_player_characters_.Find(client_index))
	{
		(*player)->player_character_state_->action_state_.input_direction.Yaw = input_dir;
		(*player)->player_character_state_->action_state_.input_bitmask = input_data;

		
		ACTION_PLAYER_PACKET packet;
		packet.packet_id = (int)PACKET_ID::SC_PLAYER_REMOTABLE_ACTION_CMD;
		packet.guid = client_index;
		packet.current_direction =(*player)->player_character_state_->character_state_.character_direction.Yaw;
		packet.current_position = (*player)->player_character_state_->character_state_.character_position;
		packet.input_direction = (*player)->player_character_state_->action_state_.input_direction.Yaw;
		packet.input_data = (*player)->player_character_state_->action_state_.input_bitmask;
		for(auto online_player_pair : online_player_characters_)
		{
			const auto& online_player_index = (online_player_pair.Key);
			const auto& online_player = (online_player_pair.Value);

			if(client_index != online_player_index)
			{
				game_instance_->SendPacketFunc(online_player_index, packet.packet_length, (char*)&packet);
			}
		}
	}
}

void AAZPlayerController_Server::EquipPlayer_Origin(int32 client_index, int32 item_id)
{
	if(const auto player = online_player_characters_.Find(client_index))
	{
		(*player)->ChangeEquipment(item_id);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_Server::AddPlayerState_Remotable(int32 client_index)
{
	
}

void AAZPlayerController_Server::RemovePlayerState_Remotable(int32 client_index)
{
	
}

void AAZPlayerController_Server::AddPlayer_Remotable(int32 client_index)
{
	
}

void AAZPlayerController_Server::RemovePlayer_Remotable(int32 client_index)
{
	
}

void AAZPlayerController_Server::ActionPlayer_Remotable(int32 client_index)
{
	
}

void AAZPlayerController_Server::EquipPlayer_Remotable(int32 client_index)
{
	
}

void AAZPlayerController_Server::UpdatePlayerState_Remotable(int32 client_index)
{
	
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAZPlayerController_Server::SendPlayerSimulationResult(int32 client_index)
{
	//해당 인덱스의 플레이어의 상태보내기?
	if(const auto player = online_player_characters_.Find(client_index))
	{
		if(auto player_state = (*player)->GetPlayerState<AAZPlayerState_Client>())
		{
			//클라는 해당정보를 받고 보간적용?
		}
	}
}
