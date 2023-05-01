// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/AZPlayer.h"
#include "PlayerState/AZPlayerState.h"
#include "Actor/AZSocketActor.h"
#include "AnimInstance/AZAnimInstance_Player.h"
//#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>

#include "AZPlayer_Playable.h"
#include "Engine/SkeletalMeshSocket.h"


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



	
	//TODO: BeginPlay에서 로드할 것(현재는 임시)
	//TODO: 임시 테이블 연동필요(현재는 기본 파츠로 생성)
	//Head->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/LHSCharacter/Mesh/FHelmet/SK_FHelmet_500"), NULL, LOAD_None, NULL));//헤어와 헬멧 구분
	Hair->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FHair/SK_FHair_121"), nullptr, LOAD_None, nullptr));
	//Face->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FFace/SK_FFace"), nullptr, LOAD_None, nullptr));
	Body->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FBody/SK_FBody_500"), nullptr, LOAD_None, nullptr));
	Arm->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FArm/SK_FArm_500"), nullptr, LOAD_None, nullptr));
	Leg->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FLeg/SK_FLeg_500"), nullptr, LOAD_None, nullptr));
	Waist->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FWaist/SK_FWaist_500"), nullptr, LOAD_None, nullptr));
	SetCombineSKMeshParts(true);
}

void AAZPlayer::PostInitProperties()
{
	Super::PostInitProperties();
	
}

void AAZPlayer::BeginPlay()
{
	Super::BeginPlay();
	PRINT_FUNCTION();
	const auto SocketItem = GetWorld()->SpawnActor<AAZSocketActor>(FVector::ZeroVector,FRotator::ZeroRotator);
	if(SocketItem != nullptr) SocketItem->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Socket_Back"));
	SocketItem->SocketObject->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/WP00/SK_WP00_302"), nullptr, LOAD_None, nullptr));
	//Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform,TEXT("Socket_Back"));
}

void AAZPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
}

void AAZPlayer::BeginDestroy()
{
	Super::BeginDestroy();
	
}

void AAZPlayer::UpdateSKMeshParts()
{
	if(auto AZPlayerState = Cast<AAZPlayerState>(GetPlayerState()))
	{
		//AZPlayerState->EquipmentState.BodyItemID
	}
	//경로만들어서 가져오나
	//TODO: 데이터테이블로 처리하기(현재는 기본 파츠로)
	//Head->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(NULL, TEXT("/Game/LHSCharacter/Mesh/FHead/SK_FHead_???"), NULL, LOAD_None, NULL));//헤어와 헬멧 구분
	
	//Hair->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FHair/SK_FHair_121"), nullptr, LOAD_None, nullptr));
	//Face->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FFace/SK_FFace"), nullptr, LOAD_None, nullptr));
	//Body->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FBody/SK_FBody_500"), nullptr, LOAD_None, nullptr));
	//Arm->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FArm/SK_FArm_500"), nullptr, LOAD_None, nullptr));
	//Leg->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FLeg/SK_FLeg_500"), nullptr, LOAD_None, nullptr));
	//Waist->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FWaist/SK_FWaist_500"), nullptr, LOAD_None, nullptr));
	
	//TODO: 무기 소켓에 붙혀서 사용해보기
	//
	
	SetCombineSKMeshParts(true);
}

void AAZPlayer::SetCombineSKMeshParts(bool bForceUpdate)
{
	if(Head) Head->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Hair) Hair->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	
	if(Face) Face->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	
	if(Body) Body->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Arm) Arm->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Leg) Leg->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
	if(Waist) Waist->SetLeaderPoseComponent(GetMesh(),bForceUpdate);
}


