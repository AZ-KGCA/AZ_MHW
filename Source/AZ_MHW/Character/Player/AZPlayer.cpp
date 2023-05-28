// Fill out your copyright notice in the Description page of Project Settings.

#include "AZPlayer.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
#include "AZ_MHW/Actor/AZSocketActor.h"
#include "AZ_MHW/Manager/AZPlayerMgr.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"

#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include <Components/SkeletalMeshComponent.h>
#include <Components/CapsuleComponent.h>

AAZPlayer::AAZPlayer()
{
    // Generic Team Agent Interface
    SetGenericTeamId(uint8(EObjectType::Player));

	// Set object collision settings
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("AZPlayer"), true);
	
	PrimaryActorTick.bStartWithTickEnabled = true;
	//캡슐 크기조정
	//GetCapsuleComponent()->InitCapsuleSize();
	const float capsule_half_height = GetCapsuleComponent()->GetScaledCapsuleHalfHeight()+2.f;
	GetMesh()->AddLocalOffset(FVector(0.0f,0.0f,-capsule_half_height));

	//Animation 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> bone_skeletal_mesh_asset(TEXT("/Game/AZ/Character/Mesh/FBone/SK_FBone"));
	GetMesh()->SetSkeletalMesh(bone_skeletal_mesh_asset.Object);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> face_skeletal_mesh_asset(TEXT("/Game/AZ/Character/Mesh/FFace/SK_FFace"));
	face_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	face_mesh_->SetupAttachment(GetMesh());
	face_mesh_->SetSkeletalMeshAsset(face_skeletal_mesh_asset.Object);
	if(face_mesh_) face_mesh_->SetLeaderPoseComponent(GetMesh(),true);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> animinstance_blueprint_asset(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_Player"));
	GetMesh()->SetAnimInstanceClass(animinstance_blueprint_asset.Class);

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


	////////////////////////////////////////////////////////////////////FX
	face_fx_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face_FX"));
	face_fx_mesh_->SetupAttachment(GetMesh());
	face_fx_mesh_->SetSkeletalMeshAsset(face_skeletal_mesh_asset.Object);
	if(face_fx_mesh_) face_fx_mesh_->SetLeaderPoseComponent(GetMesh(),true);
	
	head_fx_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head_FX"));
	head_fx_mesh_->SetupAttachment(GetMesh());
	hair_fx_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair_FX"));
	hair_fx_mesh_->SetupAttachment(GetMesh());
	body_fx_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body_FX"));
	body_fx_mesh_->SetupAttachment(GetMesh());
	arm_fx_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm_FX"));
	arm_fx_mesh_->SetupAttachment(GetMesh());
	waist_fx_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist_FX"));
	waist_fx_mesh_->SetupAttachment(GetMesh());
	leg_fx_mesh_ = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg_FX"));
	leg_fx_mesh_->SetupAttachment(GetMesh());
}

void AAZPlayer::BeginPlay()
{
	Super::BeginPlay();
	game_instance_ = Cast<UAZGameInstance>(GetWorld()->GetGameInstance());
	
	//투명 머테리얼으로 원본 가리기
	if(auto material_asset =LoadObject<UMaterialInterface>(nullptr,TEXT("/Game/AZ/Character/Mesh/FBone/M_Transparent")))
	{
		GetMesh()->SetMaterial(0, material_asset);
	}
	
	//소켓 액터생성
	CreateSocketActor(TEXT("MainHand"),TEXT("Socket_Back"));
	CreateSocketActor(TEXT("SubHand"),TEXT("Socket_Back"));
	
	//플레이어 스테이트 값에 있는 메쉬와 소켓적용하기

	// Damage Interface
	OnTakeDamage.AddDynamic(this, &AAZPlayer::PostProcessDamage);
}

void AAZPlayer::Tick(float delta_seconds)
{
	Super::Tick(delta_seconds);

	//현재 플레이어 캐릭터의 월드 위치와 월드 방향 갱신
	if(player_character_state_)
	{
		player_character_state_->character_state_.character_direction = RootComponent->GetComponentRotation();
		player_character_state_->character_state_.character_position = RootComponent->GetComponentLocation();
	}
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

	if(hair_fx_mesh_) hair_fx_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(head_fx_mesh_) head_fx_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(body_fx_mesh_) body_fx_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(arm_fx_mesh_) arm_fx_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(leg_fx_mesh_) leg_fx_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
	if(waist_fx_mesh_) waist_fx_mesh_->SetLeaderPoseComponent(GetMesh(),is_force_update);
}

void AAZPlayer::SetSKMeshParts()
{
	if(player_character_state_)
	{
		if(player_character_state_->equipment_state_.head_item_id == 12500)
		{
			if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.hair_item_id))
			{
				hair_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
				head_mesh_->SetSkeletalMesh(nullptr);
				hair_fx_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
				head_fx_mesh_->SetSkeletalMesh(nullptr);
			}
		}
		else
		{
			if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.head_item_id))
			{
				head_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
				hair_mesh_->SetSkeletalMesh(nullptr);
				head_fx_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
				hair_fx_mesh_->SetSkeletalMesh(nullptr);
			}
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.body_item_id))
		{
			body_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
			body_fx_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.arm_item_id))
		{
			arm_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
			arm_fx_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.leg_item_id))
		{
			leg_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
			leg_fx_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		if(auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.waist_item_id))
		{
			waist_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
			waist_fx_mesh_->SetSkeletalMeshAsset(skeletal_mesh_asset);
		}
		CombineSKMeshParts(true);
	}
}

void AAZPlayer::SetSKMeshSocket()
{
	if(player_character_state_)
	{
		if(const auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.first_weapon_item_id))
		{
			if(auto socket_actor = character_sockets_map_.Find(TEXT("MainHand")))
			{
				(*socket_actor)->socket_mesh_asset_->SetSkeletalMeshAsset(skeletal_mesh_asset);
				(*socket_actor)->socket_fx_mesh_asset_->SetSkeletalMeshAsset(skeletal_mesh_asset);
			}
		}
		else
		{
			if(auto socket_actor = character_sockets_map_.Find(TEXT("MainHand")))
			{
				(*socket_actor)->socket_mesh_asset_->SetSkeletalMeshAsset(nullptr);
				(*socket_actor)->socket_fx_mesh_asset_->SetSkeletalMeshAsset(nullptr);
			}
		}
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		if(const auto skeletal_mesh_asset = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(player_character_state_->equipment_state_.second_weapon_item_id))
		{
			if(auto socket_actor = character_sockets_map_.Find(TEXT("SubHand")) )
			{
				(*socket_actor)->socket_mesh_asset_->SetSkeletalMeshAsset(skeletal_mesh_asset);
				(*socket_actor)->socket_fx_mesh_asset_->SetSkeletalMeshAsset(skeletal_mesh_asset);
			}
		}
		else
		{
			if(auto socket_actor = character_sockets_map_.Find(TEXT("SubHand")) )
			{
				(*socket_actor)->socket_mesh_asset_ = nullptr;
				(*socket_actor)->socket_fx_mesh_asset_ = nullptr;
			}
		}
	}
}

//TEMP 상수경로
//void AAZPlayer::SetMeshEfxMaterial(FString material_name_path)
void AAZPlayer::SetSKMeshEfxMaterial()
{
	//if(auto material_asset =LoadObject<UMaterialInterface>(nullptr, *material_name_path))
	if(auto material_asset =LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/AZ/BodyAura/M_BodyAura")))
	{
		face_fx_mesh_->SetMaterial(0, material_asset);
		
		body_fx_mesh_->SetMaterial(0, material_asset);
		arm_fx_mesh_->SetMaterial(0, material_asset);
		head_fx_mesh_->SetMaterial(0, material_asset);
		leg_fx_mesh_->SetMaterial(0, material_asset);
		waist_fx_mesh_->SetMaterial(0, material_asset);
		hair_fx_mesh_->SetMaterial(0, material_asset);

		if(auto socket_actor = character_sockets_map_.Find(TEXT("MainHand")))
		{
			if((*socket_actor)->socket_fx_mesh_asset_)
			{
				(*socket_actor)->socket_fx_mesh_asset_->SetMaterial(0, material_asset);
			}
		}
		if(auto socket_actor = character_sockets_map_.Find(TEXT("SubHand")) )
		{
			if((*socket_actor)->socket_fx_mesh_asset_)
			{
				(*socket_actor)->socket_fx_mesh_asset_->SetMaterial(0, material_asset);
			}
		}
	}
}

//TEMP 상수경로
void AAZPlayer::SetNiagaraEfx()
{
	auto niagara_asset =LoadObject<UNiagaraSystem>(nullptr, TEXT("/Game/AZ/BodyAura/NS_AZCharge"));

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		niagara_asset,
		GetActorLocation()+FVector(0,0,20),
		FRotator::ZeroRotator,
		FVector::OneVector,
		false,
		true,
		ENCPoolMethod::None,
		true);
	
}

void AAZPlayer::SetEnableSKMeshEfx(bool on_off)
{
	face_fx_mesh_->SetVisibility(on_off);
	
	body_fx_mesh_->SetVisibility(on_off);
	arm_fx_mesh_->SetVisibility(on_off);
	head_fx_mesh_->SetVisibility(on_off);
	leg_fx_mesh_->SetVisibility(on_off);
	waist_fx_mesh_->SetVisibility(on_off);
	hair_fx_mesh_->SetVisibility(on_off);

	if(auto socket_actor = character_sockets_map_.Find(TEXT("MainHand")))
	{
		if((*socket_actor)->socket_fx_mesh_asset_)
		{
			(*socket_actor)->socket_fx_mesh_asset_->SetVisibility(on_off);
		}
	}
	if(auto socket_actor = character_sockets_map_.Find(TEXT("SubHand")) )
	{
		if((*socket_actor)->socket_fx_mesh_asset_)
		{
			(*socket_actor)->socket_fx_mesh_asset_->SetVisibility(on_off);
		}
	}
	SetNiagaraEfx();
}

void AAZPlayer::ChangeEquipment(int32 item_id)
{
	auto material_asset =LoadObject<UMaterialInterface>(nullptr, TEXT("/Game/AZ/BodyAura/MI_BodyAura"));
	if(material_asset == nullptr) return;

	//TODO item_id = weapon_type가져오기
	if(1000 < item_id && item_id < 8001)
	{
		ChangeSocketMesh(TEXT("MainHand"),item_id);
		if(auto socket_actor = character_sockets_map_.Find(TEXT("MainHand")))
		{
			if((*socket_actor)->socket_fx_mesh_asset_)
			{
				(*socket_actor)->socket_fx_mesh_asset_->SetMaterial(0,material_asset);
				player_character_state_->equipment_state_.first_weapon_item_id = item_id;
			}
		}
	}
	if(10000 < item_id && item_id < 12600)
	{
		if(auto skeletal_mesh = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(item_id))
		{
			if(item_id <= 10500)
			{
				body_mesh_->SetSkeletalMesh(skeletal_mesh);
				body_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				body_fx_mesh_->SetSkeletalMesh(skeletal_mesh);
				body_fx_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				body_fx_mesh_->SetMaterial(0, material_asset);
				player_character_state_->equipment_state_.body_item_id = item_id;
			}
			else if(item_id <= 11000)
			{
				leg_mesh_->SetSkeletalMesh(skeletal_mesh);
				leg_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				leg_fx_mesh_->SetSkeletalMesh(skeletal_mesh);
				leg_fx_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				leg_fx_mesh_->SetMaterial(0, material_asset);
				player_character_state_->equipment_state_.leg_item_id = item_id;
			}
			else if(item_id <= 11500)
			{
				arm_mesh_->SetSkeletalMesh(skeletal_mesh);
				arm_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				arm_fx_mesh_->SetSkeletalMesh(skeletal_mesh);
				arm_fx_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				arm_fx_mesh_->SetMaterial(0, material_asset);
				player_character_state_->equipment_state_.arm_item_id = item_id;
			}
			else if(item_id <= 12000)
			{
				waist_mesh_->SetSkeletalMesh(skeletal_mesh);
				waist_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				waist_fx_mesh_->SetSkeletalMesh(skeletal_mesh);
				waist_fx_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				waist_fx_mesh_->SetMaterial(0, material_asset);
				player_character_state_->equipment_state_.waist_item_id = item_id;
			}
			else if(item_id <= 12500)
			{
				head_mesh_->SetSkeletalMesh(skeletal_mesh);
				head_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				head_fx_mesh_->SetSkeletalMesh(skeletal_mesh);
				head_fx_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				head_fx_mesh_->SetMaterial(0, material_asset);
				player_character_state_->equipment_state_.head_item_id = item_id;
			}
			else if(item_id < 12600)
			{
				hair_mesh_->SetSkeletalMesh(skeletal_mesh);
				hair_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				hair_fx_mesh_->SetSkeletalMesh(skeletal_mesh);
				hair_fx_mesh_->SetLeaderPoseComponent(GetMesh(),true);
				hair_fx_mesh_->SetMaterial(0, material_asset);
				player_character_state_->equipment_state_.hair_item_id = item_id;
			}
		}
	}
}

void AAZPlayer::CreateSocketActor(FName new_socket_actor_name, FName in_socket_name)
{
	if(const auto exist_socket_actor = character_sockets_map_.Find(new_socket_actor_name))
	{
		(*exist_socket_actor)->SetSocketComponent(in_socket_name);
	}
	else
	{
		if(const auto socket_actor = GetWorld()->SpawnActor<AAZSocketActor>(FVector::ZeroVector, FRotator::ZeroRotator))
		{
			socket_actor->SetSocketComponent(in_socket_name,GetMesh());
			character_sockets_map_.Add(new_socket_actor_name, socket_actor);
		}
	}
}

void AAZPlayer::ChangeSocketMesh(FName socket_actor_name, int32 item_id)
{
	if(const auto socket_actor = character_sockets_map_.Find(socket_actor_name))
	{
		if(const auto item_mesh = UAZGameSingleton::instance()->player_mgr_->GetSkeletalMesh(item_id))
		{
			(*socket_actor)->socket_mesh_asset_->SetSkeletalMesh(item_mesh);
			(*socket_actor)->socket_fx_mesh_asset_->SetSkeletalMesh(item_mesh);
		}
	}
}

void AAZPlayer::ChangeSocketSlot(FName socket_actor_name, FName in_socket_name)
{
	if(const auto socket_actor = character_sockets_map_.Find(socket_actor_name))
	{
		(*socket_actor)->SetSocketComponent(in_socket_name);
	}
}


//TODO 데미지처리 오리진으로 옮기기
float AAZPlayer::ApplyDamage_Implementation(AActor* damaged_actor, const FHitResult hit_result, FAttackInfo attack_info)
{
	// Process Damage on damaged actor
	AAZMonster* monster = Cast<AAZMonster>(damaged_actor);
	return monster->ProcessDamage(this, hit_result, attack_info);
}

//TODO 데미지처리 오리진으로 옮기기
float AAZPlayer::ProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	return Super::ProcessDamage(damage_instigator, hit_result, attack_info);
}

//TODO 데미지처리 오리진으로 옮기기 방향계산 (서버의 애니메이션쪽에서 하기->SC_UPDATE_STATE_CMD로 강제변경하기)
void AAZPlayer::PostProcessDamage(AActor* damage_instigator, const FHitResult hit_result, FAttackInfo attack_info)
{
	//Origin과 Playable
	AAZMonster* instigator_monster = Cast<AAZMonster>(damage_instigator);
	if (!instigator_monster)
	{
		UE_LOG(AZMonster, Warning, TEXT("[AAZPlayer] Damage dealt by non-AZMonster actor %s"), *instigator_monster->GetName());
		return;
	}
	
	//함수화하기
	const FVector forward_direction = GetActorForwardVector();
	const FVector from_hit_direction = (instigator_monster->GetActorLocation());
	const FVector to_hit_direction = (from_hit_direction - GetActorLocation()).GetSafeNormal();

	const double cos_theta = FVector::DotProduct(forward_direction,to_hit_direction);
	double theta = FMath::Acos(cos_theta);
	theta = FMath::RadiansToDegrees(theta);

	const FVector cross_vector = FVector::CrossProduct(forward_direction,to_hit_direction);
	if(cross_vector.Z<0)
	{
		theta *=-1.f;
	}
	//피해 타입(약(살짝경직),중(날라가지만 자세),강(날라가서구르기))에 따라 강제회전타입(앞뒤 넉백 에어본)도 넣기
	//Default Back
	if(theta >=-45.f && theta < 45.f)
	{
		//Front
		UE_LOG(AZ_DAMAGE,Error,TEXT("Front"));
		//Cast<UAZAinmInstance_Player>(GetMesh()->GetAnimInstance())->SetMontageName(TEXT(Hit Front));
	}
	else if(theta >= -135.f && theta <-45.f)
	{
		//Left
		UE_LOG(AZ_DAMAGE,Error,TEXT("Left"));
	}
	else if(theta >=45.f && theta < 135.f)
	{
		//Right
		UE_LOG(AZ_DAMAGE,Error,TEXT("Right"));
	}
	
	
	if(const auto player_state = GetPlayerState<AAZPlayerState_Client>())
	{
	    //TODO 공격타입별로 데미지 계산, attack_info.damage_array에 TTuple<EDamageType, int32> 꼴로 있습니다
		player_state->character_state_.current_health_point -= attack_info.GetDamageTotal();
		if(player_state->character_state_.current_health_point <= 0)
		{
			player_state->character_state_.current_health_point = 0;
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Player Health: %d"),
				player_state->character_state_.current_health_point));
		}
	}
}