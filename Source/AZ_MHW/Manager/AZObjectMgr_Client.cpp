// Copyright Team AZ. All Rights Reserved.


#include "AZObjectMgr_Client.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"

UAZObjectMgr_Client::UAZObjectMgr_Client()
{
}

void UAZObjectMgr_Client::BeginDestroy()
{
	UObject::BeginDestroy();
	object_map_.Empty();
}

bool UAZObjectMgr_Client::AddObject(int32 object_serial, AAZCharacter* character)
{
	if (object_map_.Find(object_serial))
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Client] Serial No: %d already exists!"), object_serial);
		return false;
	}
	else
	{
		object_map_.Add(object_serial, character);
		UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Client] %s added. Serial No: %d"), *character->GetName(), object_serial);
		return true;
	}
}

bool UAZObjectMgr_Client::RemoveObject(int32 object_serial)
{
	if (object_map_.Remove(object_serial))
	{
		UE_LOG(AZ, Log, TEXT("[UAZObjectMgr_Client] Serial No: %d is removed"), object_serial);
		return true;
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Client] Serial No: %d does not exist!"), object_serial);
		return false;
	}
}

bool UAZObjectMgr_Client::SpawnMonster(int32 object_serial, int32 monster_id, FVector spawn_location, FRotator spawn_rotation)
{
	// 포인터 풀릴 것 같은데 안풀리는지 체크 TODO
	// Retrieve the blueprint path of the monster to spawn
	const FName monster_name = UAZGameSingleton::instance()->monster_mgr_->GetMonsterName(monster_id);
	if (monster_name == NAME_None) return false;
	const FString bp_path = FString::Printf(TEXT("/Script/Engine.Blueprint'/Game/AZ/Monsters/%s/Blueprints/BP_%s.BP_%s'"),
		*monster_name.ToString(), *monster_name.ToString(), *monster_name.ToString());
	
	// Spawn a monster from the blueprint with the received transform
	const UBlueprint* monster_bp = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), this, *bp_path));
	AAZMonster_Client* monster = GetWorld()->SpawnActor<AAZMonster_Client>(monster_bp->GeneratedClass, spawn_location, spawn_rotation);

	AddObject(object_serial, monster);
	return true;
}

AAZCharacter* UAZObjectMgr_Client::GetObject(int32 object_serial)
{
	if (const auto object = object_map_.Find(object_serial))
	{
		return *object;
	}
	else
	{
		UE_LOG(AZ, Error, TEXT("[UAZObjectMgr_Client][#%d] Object not found!"), object_serial);
		return nullptr;
	}
}

AAZMonster_Client* UAZObjectMgr_Client::GetMonster(int32 object_serial)
{
	if (const auto monster = GetObject(object_serial))
	{
		return static_cast<AAZMonster_Client*>(monster);
	}
	else
	{
		return nullptr;
	}
}

AAZPlayer* UAZObjectMgr_Client::GetPlayer(int32 object_serial)
{
	if (const auto player = GetObject(object_serial))
	{
		return static_cast<AAZPlayer*>(player);
	}
	else
	{
		return nullptr;
	}
}

