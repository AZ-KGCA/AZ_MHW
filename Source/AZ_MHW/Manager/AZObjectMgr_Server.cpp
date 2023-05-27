﻿// Copyright Team AZ. All Rights Reserved.


#include "AZObjectMgr_Server.h"
#include "AZ_MHW/Manager/AZMonsterMgr.h"
#include "AZ_MHW/Character/AZCharacter.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Character/Player/AZPlayer_Origin.h"

UAZObjectMgr_Server::UAZObjectMgr_Server()
{
	object_serial_ = -1;
	
	// TEMP
	spawn_array_.Add(FMonsterSpawnInfo(0, EBossRank::Low, FVector(-100,11000,200), FRotator::ZeroRotator));
	//spawn_array_.Add(FMonsterSpawnInfo(0, EBossRank::Low, FVector(-1200,7000,200), FRotator::ZeroRotator));
	//spawn_array_.Add(FMonsterSpawnInfo(0, EBossRank::Low, FVector(3000,7000,200), FRotator::ZeroRotator));
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
	if (AAZMonster* monster = Cast<AAZMonster>(character))
	{
		spawned_monsters_array_.Add(monster);
	}
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

AAZMonster* UAZObjectMgr_Server::SpawnMonster(int32 monster_id, EBossRank rank, FVector spawn_location, FRotator spawn_rotation)
{
	/* legacy: spawn from blueprint asset
		// Retrieve the blueprint path of the monster to spawn
		const FName monster_name = UAZGameSingleton::instance()->monster_mgr_->GetMonsterName(monster_id);
		if (monster_name == NAME_None) return false;
		const FString bp_path = FString::Printf(TEXT("/Script/Engine.Blueprint'/Game/AZ/Monsters/%s/Blueprints/BP_%s.BP_%s'"),
			*monster_name.ToString(), *monster_name.ToString(), *monster_name.ToString());
		
		UWorld* world = GetOuter()->GetWorld();
		if (!world) return nullptr;
		
		// Spawn a monster from the blueprint with the received transform
		const UBlueprint* monster_bp = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), this, *bp_path));
		AAZMonster* monster = world->SpawnActor<AAZMonster>(monster_bp->GeneratedClass, spawn_location, spawn_rotation);
	*/

	UWorld* world = GetOuter()->GetWorld();
	if (!world) return nullptr;

	const FTransform spawn_transform(spawn_rotation, spawn_location);
	AAZMonster* monster = world->SpawnActorDeferred<AAZMonster>(AAZMonster::StaticClass(),
		spawn_transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	monster->Init(monster_id, rank);
	monster->FinishSpawning(spawn_transform);
	AddObject(monster);
		
	return monster;
}

AAZCharacter* UAZObjectMgr_Server::GetObject(int32 object_serial)
{
	if (const auto object = object_map_.Find(object_serial))
	{
		return object->Get();
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

AAZPlayer_Origin* UAZObjectMgr_Server::GetPlayer(int32 object_serial)
{
	if (const auto player = GetObject(object_serial_))
	{
		return static_cast<AAZPlayer_Origin*>(player);
	}
	else
	{
		return nullptr;
	}
}

// 따로 빼야됨
TArray<FMonsterSpawnInfo>* UAZObjectMgr_Server::GetSpawnArray()
{
	return &spawn_array_;
}

void UAZObjectMgr_Server::SpawnAllMonsters()
{
	if (spawn_finished_) return;
	for (auto info : spawn_array_)
	{
		AAZMonster* new_monster = SpawnMonster(
			info.monster_id, info.rank, info.location, info.rotation);
	}
	spawn_finished_ = true;	
}
