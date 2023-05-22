// Copyright Team AZ. All Rights Reserved.


#include "AZObjectMgr_Server.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"

UAZObjectMgr_Server::UAZObjectMgr_Server()
{
	object_serial_ = -1;
}

void UAZObjectMgr_Server::BeginDestroy()
{
	UObject::BeginDestroy();
	object_map_.Empty();
}

void UAZObjectMgr_Server::AddObject(AAZCharacter* character)
{
	object_map_.Add(++object_serial_, character);
	character->object_serial_ = object_serial_;
	UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Server] %s added. Serial No: %d"), *character->GetName(), object_serial_);
}

bool UAZObjectMgr_Server::RemoveObject(int32 object_serial)
{
	if (object_map_.Remove(object_serial))
	{
		UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Server] Serial No: %d is removed"), object_serial);
		return true;
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Server] Serial No: %d does not exist!"), object_serial);
		return false;
	}
}

bool UAZObjectMgr_Server::SpawnMonster(int32 monster_id, FVector spawn_location, FRotator spawn_rotation)
{
	// 포인터 풀릴 것 같은데 안풀리는지 체크 TODO
	// Retrieve the blueprint path of the monster to spawn
	const FName monster_name = UAZGameSingleton::instance()->monster_mgr_->GetMonsterName(monster_id);
	if (monster_name == NAME_None) return false;
	const FString bp_path = FString::Printf(TEXT("/Script/Engine.Blueprint'/Game/AZ/Monsters/%s/Blueprints/BP_%s.BP_%s'"),
		*monster_name.ToString(), *monster_name.ToString(), *monster_name.ToString());
	
	// Spawn a monster from the blueprint with the received transform
	const UBlueprint* monster_bp = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), this, *bp_path));
	AAZMonster* monster = GetWorld()->SpawnActor<AAZMonster>(monster_bp->GeneratedClass, spawn_location, spawn_rotation);

	AddObject(monster);
	return true;
}

AAZCharacter* UAZObjectMgr_Server::GetObject(int32 object_serial)
{
	if (const auto object = object_map_.Find(object_serial))
	{
		return *object;
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Server][#%d] Object not found!"), object_serial);
		return nullptr;
	}
}

AAZMonster* UAZObjectMgr_Server::GetMonster(int32 object_serial)
{
	if (const auto monster = GetObject(object_serial_))
	{
		return static_cast<AAZMonster*>(monster);
	}
	else
	{
		return nullptr;
	}
}

AAZPlayer* UAZObjectMgr_Server::GetPlayer(int32 object_serial)
{
	if (const auto player = GetObject(object_serial_))
	{
		return static_cast<AAZPlayer*>(player);
	}
	else
	{
		return nullptr;
	}
}

