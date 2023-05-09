// Fill out your copyright notice in the Description page of Project Settings.


#include "AZPlayer.h"
#include "AZ_MHW/Character/Player/AZPlayer_Playable.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/PlayerState/AZPlayerState.h"
#include "AZ_MHW/Actor/AZSocketActor.h"
#include "AZ_MHW/Manager/AZPlayerAssetMgr.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include <Components/SkeletalMeshComponent.h>

//#include "AZ_MHW/Manager/AZTableMgr.h"
//#include <Components/CapsuleComponent.h>


AAZPlayer::AAZPlayer()
{
    // Generic Team Agent Interface
    SetGenericTeamId(uint8(EObjectType::Player));
    
	PrimaryActorTick.bStartWithTickEnabled = true;
	//캡슐 크기조정
	//GetCapsuleComponent()->InitCapsuleSize();
	//(GetMesh()-> 위치 Z-90.f)//root의 바닥에 붙히기

	//Animation 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletal_mesh_asset(TEXT("/Game/AZ/Character/Mesh/FBone/SK_FBone"));
	GetMesh()->SetSkeletalMesh(skeletal_mesh_asset.Object);

	//Base Female Face
	face_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	face_mesh_->SetupAttachment(GetMesh());
	face_mesh_->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FFace/SK_FFace"), nullptr, LOAD_None, nullptr));
	if(face_mesh_) face_mesh_->SetLeaderPoseComponent(GetMesh(),true);
	
	if(Cast<AAZPlayer_Playable>(this))
	{
		static ConstructorHelpers::FClassFinder<UAnimInstance> anim_instance(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_PlayerPlayable"));
		GetMesh()->SetAnimInstanceClass(anim_instance.Class);
		//BodyAnimInstanceClass = Cast<UAZAnimInstance_Player>(ABPAnimInstance.Class);
	}
	else
	{
		static ConstructorHelpers::FClassFinder<UAnimInstance> anim_instance(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_PlayerRemotable"));
		GetMesh()->SetAnimInstanceClass(anim_instance.Class);
		//BodyAnimInstanceClass = Cast<UAZAnimInstance_Player>(ABPAnimInstance.Class);
	}
	

	head_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	head_mesh_->SetupAttachment(GetMesh());
	hair_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	hair_mesh_->SetupAttachment(GetMesh());
	body_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	body_mesh_->SetupAttachment(GetMesh());
	arm_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	arm_mesh_->SetupAttachment(GetMesh());
	waist_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	waist_mesh_->SetupAttachment(GetMesh());
	leg_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg"));
	leg_mesh_->SetupAttachment(GetMesh());
}

void AAZPlayer::PostInitProperties()
{
	Super::PostInitProperties();
	
}

void AAZPlayer::BeginPlay()
{
	Super::BeginPlay();

	//투명 머테리얼
	if(auto material_asset =LoadObject<UMaterialInterface>(nullptr,TEXT("/Game/AZ/Character/Mesh/FBone/M_Transparent")))
	{
		GetMesh()->SetMaterial(0, material_asset);
	}

	//소켓 액터생성
	this->CreateSocketActor(TEXT("MainHand"),TEXT("Socket_Back"));
	this->CreateSocketActor(TEXT("SubHand"),TEXT("Socket_Back"));
	
	SetSKMeshParts();
	SetSKMeshSocket();

	// Damage Interface
	OnTakeDamage.AddDynamic(this, &AAZPlayer::PostProcessDamage);
}

void AAZPlayer::PossessedBy(AController* new_controller)
{
	Super::PossessedBy(new_controller);
	
}

void AAZPlayer::BeginDestroy()
{
	// Damage Interface
	OnTakeDamage.RemoveDynamic(this, &AAZPlayer::PostProcessDamage);
	
	Super::BeginDestroy();
}

void AAZPlayer::CombineSKMeshParts(bool is_force_update)
{
	if(hair_mesh_) hair_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(head_mesh_) head_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(body_mesh_) body_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(arm_mesh_) arm_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(leg_mesh_) leg_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(waist_mesh_) waist_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
}

void AAZPlayer::SetSKMeshParts()
{
	if(auto player_state = Cast<AAZPlayerState>(GetPlayerState()))
	{
		if(player_state->equipment_state_.head_item_id == 12500)
		{
			if(auto SK = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.hair_item_id))
			{
				hair_mesh_->SetSkeletalMeshAsset(SK);
			}
		}
		else
		{
			if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.head_item_id))
			{
				head_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
			}
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.body_item_id))
		{
			body_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.arm_item_id))
		{
			arm_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.leg_item_id))
		{
			leg_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.waist_item_id))
		{
			waist_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}

		CombineSKMeshParts(true);
	}
}

void AAZPlayer::SetSKMeshSocket()
{
	if(const auto player_state = Cast<AAZPlayerState>(GetPlayerState()))
	{
		if(const auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.first_weapon_item_id))
		{
			(*character_sockets_map_.Find(TEXT("MainHand")))->socket_mesh_asset_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		else
		{
			(*character_sockets_map_.Find(TEXT("MainHand")))->socket_mesh_asset_ = nullptr;
		}
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		if(const auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(player_state->equipment_state_.second_weapon_item_id))
		{
			(*character_sockets_map_.Find(TEXT("SubHand")))->socket_mesh_asset_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		else
		{
			(*character_sockets_map_.Find(TEXT("SubHand")))->socket_mesh_asset_ = nullptr;
		}
	}
}


void AAZPlayer::ChangeEquipmentMesh(int32 item_id)
{
	if(10000 < item_id && item_id < 12600)
	{
		if(auto skeletal_mesh = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(item_id))
		{
			if(item_id <= 10500)
			{
				body_mesh_->SetSkeletalMesh(skeletal_mesh);
				body_mesh_->SetLeaderPoseComponent(GetMesh(),true);
			}
			else if(item_id <= 11000)
			{
				leg_mesh_->SetSkeletalMesh(skeletal_mesh);
				leg_mesh_->SetLeaderPoseComponent(GetMesh(),true);
			}
			else if(item_id <= 11500)
			{
				arm_mesh_->SetSkeletalMesh(skeletal_mesh);
				arm_mesh_->SetLeaderPoseComponent(GetMesh(),true);
			}
			else if(item_id <= 12000)
			{
				waist_mesh_->SetSkeletalMesh(skeletal_mesh);
				waist_mesh_->SetLeaderPoseComponent(GetMesh(),true);
			}
			else if(item_id <= 12500)
			{
				head_mesh_->SetSkeletalMesh(skeletal_mesh);
				head_mesh_->SetLeaderPoseComponent(GetMesh(),true);
			}
			else if(item_id < 12600)
			{
				hair_mesh_->SetSkeletalMesh(skeletal_mesh);
				hair_mesh_->SetLeaderPoseComponent(GetMesh(),true);
			}
		}
	}
}

void AAZPlayer::CreateSocketActor(FName new_socket_actor_name, FName in_socket_name)
{
	if(auto exist_socket_actor = character_sockets_map_.Find(new_socket_actor_name))
	{
		(*exist_socket_actor)->SetSocketComponent(in_socket_name);
	}
	else
	{
		AAZSocketActor* socket_actor = GetWorld()->SpawnActor<AAZSocketActor>(FVector::ZeroVector, FRotator::ZeroRotator);
		if(socket_actor != nullptr)
		{
			socket_actor->SetSocketComponent(in_socket_name,GetMesh());
			character_sockets_map_.Add(new_socket_actor_name, socket_actor);
		}
	}
}

void AAZPlayer::ChangeSocketMesh(FName socket_actor_name, int32 item_id)
{
	if(auto socket_actor = character_sockets_map_.Find(socket_actor_name))
	{
		if(auto item_mesh = UAZGameSingleton::instance()->player_asset_mgr_->GetSkeletalMesh(item_id))
		{
			(*socket_actor)->socket_mesh_asset_->SetSkeletalMesh(item_mesh);
		}
	}
}

void AAZPlayer::ChangeSocketSlot(FName socket_actor_name, FName in_socket_name)
{
	if(auto socket_actor = character_sockets_map_.Find(socket_actor_name))
	{
		(*socket_actor)->SetSocketComponent(in_socket_name);
	}
}

float AAZPlayer::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult& hit_result,
	AController* instigator, TSubclassOf<UDamageType> damage_type_class, float base_damage)
{
	float applied_damage = base_damage;
	// TODO 여기서 예리도 등 계산하셔서 넘기면 됩니다
	
	return Super::ApplyDamage_Implementation(damaged_actor, hit_result, instigator, damage_type_class, applied_damage);
}

float AAZPlayer::ProcessDamage(const FHitResult& hit_result, AController* instigator,
	TSubclassOf<UDamageType> damage_type_class, float applied_damage)
{
	float final_damage = applied_damage;
	// TODO 여기서 받은 데미지 값에서 캐릭터의 장비, 능력치 등 고려하셔서 최종 데미지 인자값으로 넘기시면 됩니다
	// Super함수 호출은 이 함수 포함 다른 함수에서도 안 해도 되는데 ProcessDamage에서 안할 경우에는 OnTakeDamage.BroadCast 해주셔야합니다

	return Super::ProcessDamage(hit_result, instigator, damage_type_class, final_damage);
}

void AAZPlayer::PostProcessDamage(float total_damage, const UDamageType* damage_type, AController* instigator)
{
	AAZMonster* instigator_monster = Cast<AAZMonster>(instigator->GetPawn());
	if (!instigator_monster)
	{
		UE_LOG(AZMonster, Warning, TEXT("[AAZPlayer] Damage dealt by non-AZMonster actor %s"), *instigator_monster->GetName());
		return;
	}
	// TEMP Test
	static int32 health = 1000;
	health -= total_damage;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player Health: %d"), health));
	}
	
	// TODO 여기서 부가적인 데미지 프로세싱 (체력 및 상태 수정 등) 하시면 됩니다
	// Super::ProcessDamage에서 브로드캐스트 됩니다 
}

