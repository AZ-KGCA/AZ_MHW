// Copyright Team AZ. All Rights Reserved.


#include "AZPlayerController_Server.h"

#include "Character/Player/AZPlayer_Origin.h"
#include "Character/Player/AZPlayer_Playable.h"
#include "PlayerState/AZPlayerState.h"


// Sets default values
AAZPlayerController_Server::AAZPlayerController_Server()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
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

void AAZPlayerController_Server::CreateClonePlayer(int32 client_index)
{
	GetWorld()->SpawnActor<AAZPlayer_Origin>();
	// UWorld* world = GetWorld();
	// if(world)
	// {
	// 	if(const auto player_clone = world->SpawnActor<AAZPlayer_Origin>())
	// 	{
	// 		logined_player_characters_.Add(client_index, player_clone);
	// 		//db data? constant data? ->Initialize
	// 		player_clone->GetRootComponent()->SetWorldLocation(FVector(0,0,0));
	// 		player_clone->GetRootComponent()->SetWorldRotation(FRotator(0,0,0));
	// 		//db
	// 		//playerState->equipment_state(character.)//
	// 		//playerState->character_state(character.)
	// 		//player에 있는 장비데이터 적용하기함수를 처리하고
	// 		//server_to_client(character_state);
	//
	// 		//player_clone->SetPlayerState();
	// 		//리스닝 서버는 캐릭터의 생김새를 가지고 있을 필요가 없다.
	//
	// 		//player_clone->SetSKMeshSocket();
	// 		//player_clone->SetSKMeshParts();
	// 	}
	// }
}

void AAZPlayerController_Server::RemoveClonePlayer(int32 client_index)
{
	if(const auto player_clone = logined_player_characters_.Find(client_index))
	{
		logined_player_characters_.Remove(client_index);
		(*player_clone)->Destroy();//?

		//서버에서 클라로 해당 id의 원격 캐릭터 제거 명령
	}
}

void AAZPlayerController_Server::ReceivePlayerInput(int32 client_index, Input_Packet* input)
{
	if(const auto player_clone = logined_player_characters_.Find(client_index))
	{
		//캐릭터의 입력값을 클라에서 만들어서 서버에 제출하는 개념
		//서버와 클라는 같은? 입력값을 처리한다.
		
		//(*player_clone)->SetPlayerState();

		//몬스터는 클라에서 상태값을 만들어서 플레이한후에
		//상태결과값(액션명령구조체?)만 클라로 전송
		
		//캐릭터의 상태는 서버에서 만들어서 제공하지만, 이것은 원격에 전달할 상태고, 클라와 확인할 비교상태입니다.
	}
}

void AAZPlayerController_Server::SendPlayerSimulationResult(int32 client_index)
{
	//해당 인덱스의 플레이어의 상태보내기?
	if(const auto player_clone = logined_player_characters_.Find(client_index))
	{
		if(auto player_state = (*player_clone)->GetPlayerState<AAZPlayerState>())
		{
			
			//클라는 해당정보를 받고 보간적용?
		}
	}
}
