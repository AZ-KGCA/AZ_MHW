// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_Server.h"

#include "Character/Player/AZPlayer_Origin.h"
#include "Character/Player/AZPlayer_Playable.h"
#include "PlayerState/AZPlayerState_Client.h"


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

void AAZPlayerController_Server::AddPlayerState_Origin(int32 client_index)
{
	AAZPlayerState_Client* player_state_origin;
	if(auto found_player_state_origin = online_player_character_state_.Find(client_index))
	{
		player_state_origin = (*found_player_state_origin);
	}
	else
	{
		//없으면 생성
		player_state_origin = GetWorld()->SpawnActor<AAZPlayerState_Client>();
	}
	online_player_character_state_.Add(client_index,player_state_origin);
	//DB에서 가져와서 채우기
	//player_state_origin->equipment_state_.
	//player_state_origin->character_state_.
}
void AAZPlayerController_Server::RemovePlayerState_Origin(int32 client_index)
{
	
}

void AAZPlayerController_Server::AddPlayer_Origin(int32 client_index)
{
	//TODO::임시
	AddPlayerState_Origin(client_index);
	auto player_origin = GetWorld()->SpawnActor<AAZPlayer_Origin>();
	if(const auto player_state_origin = online_player_character_state_.Find(client_index))
	{
		player_origin->SetPlayerState(*player_state_origin);
		player_origin->player_character_state_ = (*player_state_origin);
		
		//player_clone->GetRootComponent()->SetWorldLocation(FVector(0,0,0));
		//player_clone->GetRootComponent()->SetWorldRotation(FRotator(0,0,0));
		
		online_player_characters_.Add(client_index,player_origin);
	}
	

	// 		//db data? constant data? ->Initialize
	// 		player_clone->GetRootComponent()->SetWorldLocation(FVector(0,0,0));
	// 		player_clone->GetRootComponent()->SetWorldRotation(FRotator(0,0,0));
	// 		//db
	// 		//playerState->equipment_state(character.)//
	// 		//playerState->character_state(character.)
	// 		//player에 있는 장비데이터 적용하기함수를 처리하고
	// 		//server_to_client(character_state);

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
	if(const auto player_origin = online_player_characters_.Find(client_index))
	{
		(*player_origin)->player_character_state_->action_state_.input_direction.Yaw = input_dir;
		(*player_origin)->player_character_state_->action_state_.input_bitmask = input_data;
		UE_LOG(AZ_TEST,Warning,TEXT("%d"),input_data);
		//캐릭터의 입력값을 클라에서 만들어서 서버에 제출하는 개념
		//서버와 클라는 같은? 입력값을 처리한다.
		
		//(*player_clone)->SetPlayerState();
	
		//몬스터는 클라에서 상태값을 만들어서 플레이한후에
		//상태결과값(액션명령구조체?)만 클라로 전송
		
		//캐릭터의 상태는 서버에서 만들어서 제공하지만, 이것은 원격에 전달할 상태고, 클라와 확인할 비교상태입니다.
	}
}

void AAZPlayerController_Server::EquipPlayer_Origin(int32 client_index, int32 pos)
{

	
}

void AAZPlayerController_Server::UpdatePlayer_Origin(int32 clinet_index)
{
	
}




void AAZPlayerController_Server::SendPlayerSimulationResult(int32 client_index)
{
	//해당 인덱스의 플레이어의 상태보내기?
	if(const auto player_origin = online_player_characters_.Find(client_index))
	{
		if(auto player_state = (*player_origin)->GetPlayerState<AAZPlayerState_Client>())
		{
			
			//클라는 해당정보를 받고 보간적용?
		}
	}
}