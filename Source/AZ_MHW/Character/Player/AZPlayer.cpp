// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/AZPlayer.h"
#include "PlayerState/AZPlayerState.h"
#include "Actor/AZSocketActor.h"
#include "AnimInstance/AZAnimInstance_Player.h"
//#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include "AZ_MHW/Util/AZUtility.h"
//#include "GameSingleton/AZGameSingleton.h"
//#include "Manager/AZTableMgr.h"
#include "GameInstance/AZGameInstance.h"
#include "Manager/AZPlayerAssetMgr.h"
#include "AZPlayer_Playable.h"


AAZPlayer::AAZPlayer()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	//캡슐 크기조정
	//GetCapsuleComponent()->InitCapsuleSize();

	//Animation 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMesh(TEXT("/Game/AZ/Character/Mesh/FBone/SK_FBone"));
	GetMesh()->SetSkeletalMesh(SKMesh.Object);
	if(Cast<AAZPlayer_Playable>(this))
	{
		static ConstructorHelpers::FClassFinder<UAnimInstance> ABPAnimInstance(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_PlayerPlayable"));
		GetMesh()->SetAnimInstanceClass(ABPAnimInstance.Class);
		BodyAnimInstanceClass = ABPAnimInstance.Class;
	}
	else
	{
		static ConstructorHelpers::FClassFinder<UAnimInstance> ABPAnimInstance(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_PlayerRemotable"));
		GetMesh()->SetAnimInstanceClass(ABPAnimInstance.Class);
		BodyAnimInstanceClass = ABPAnimInstance.Class;
	}
	
	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	Face->SetupAttachment(GetMesh());
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetMesh());
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetMesh());
	Arm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	Arm->SetupAttachment(GetMesh());
	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Waist->SetupAttachment(GetMesh());
	Leg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg"));
	Leg->SetupAttachment(GetMesh());

	//Base Female Face
	Face->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FFace/SK_FFace"), nullptr, LOAD_None, nullptr));

	
}

void AAZPlayer::PostInitProperties()
{
	Super::PostInitProperties();
	
}

void AAZPlayer::BeginPlay()
{
	Super::BeginPlay();
	PRINT_FUNCTION();
	
	FirstSocket = GetWorld()->SpawnActor<AAZSocketActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	if(FirstSocket != nullptr)
	{
		FirstSocket->SetSocketComponent(TEXT("Socket_Back"),GetMesh());
	}
	SecondSocket = GetWorld()->SpawnActor<AAZSocketActor>(FVector::ZeroVector, FRotator::ZeroRotator);
	if(SecondSocket != nullptr)
	{
		SecondSocket->SetSocketComponent(TEXT("Socket_Back"),GetMesh());
	}
	SetSKMeshParts();
	SetSKMeshSocket();
}

void AAZPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
}

void AAZPlayer::BeginDestroy()
{
	Super::BeginDestroy();
	
}

void AAZPlayer::CombineSKMeshParts(bool bForceUpdate)
{
	if(Hair) Hair->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Face) Face->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	
	if(Head) Head->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	
	if(Body) Body->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Arm) Arm->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Leg) Leg->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Waist) Waist->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
}

void AAZPlayer::SetSKMeshParts()
{
	if(auto AZPlayerState = Cast<AAZPlayerState>(GetPlayerState()))
	{
		if(AZPlayerState->EquipmentState.HeadItemID == 12500)
		{
			if(auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.HairItemID))
			{
				Hair->SetSkeletalMeshAsset(SK);
			}
		}
		else
		{
			if(auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.HeadItemID))
			{
				Head->SetSkeletalMeshAsset(SK);
			}
		}
		if(auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.BodyItemID))
		{
			Body->SetSkeletalMeshAsset(SK);
		}
		if(auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.ArmItemID))
		{
			Arm->SetSkeletalMeshAsset(SK);
		}
		if(auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.LegItemID))
		{
			Leg->SetSkeletalMeshAsset(SK);
		}
		if(auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.WaistItemID))
		{
			Waist->SetSkeletalMeshAsset(SK);
		}

		CombineSKMeshParts(true);
	}
}

void AAZPlayer::SetSKMeshSocket()
{
	if(const auto AZPlayerState = Cast<AAZPlayerState>(GetPlayerState()))
	{
		if(const auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.WeaponItemID))
		{
			FirstSocket->SocketObject->SetSkeletalMeshAsset(SK);
			FirstSocket->SetSocketComponent(FirstSocket->CurrentSocketName,GetMesh());
		}
		else
		{
			FirstSocket->SocketObject = nullptr;
		}
		///
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		///
		if(const auto SK = AZGameInstance->playerAsset_mgr->GetSkeletalMesh(AZPlayerState->EquipmentState.SecondWeaponItemID))
		{
			SecondSocket->SocketObject->SetSkeletalMeshAsset(SK);
			SecondSocket->SetSocketComponent(SecondSocket->CurrentSocketName,GetMesh());
		}
		else
		{
			SecondSocket->SocketObject = nullptr;
		}
	}
}


