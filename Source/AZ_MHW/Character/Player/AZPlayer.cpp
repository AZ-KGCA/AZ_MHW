// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/AZPlayer.h"
#include "PlayerState/AZPlayerState.h"
#include "AnimInstance/AZAnimInstance_Player.h"
//#include <Components/CapsuleComponent.h>
#include <Components/SkeletalMeshComponent.h>


AAZPlayer::AAZPlayer()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	//캡슐 크기맞게끔 줄이기
	//GetCapsuleComponent()->InitCapsuleSize();
	
	//몬스터헌터 스켈레톤 메시 스케일이 100배라서 줄이기
	//FBX Import 자체를 100배 줄이기 
	//const FVector MeshSize = FVector(0.01f,0.01f,0.01f);
	//GetMesh()->SetWorldScale3D(MeshSize);
	
	Head = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	Head->SetupAttachment(GetMesh());//CDO생성시에
	//Head->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);//Runtime중에
	Hair = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair"));
	Hair->SetupAttachment(GetMesh());
	Face = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Face"));
	Face->SetupAttachment(GetMesh());
	Body = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetMesh());
	Arm = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Arm"));
	Arm->SetupAttachment(GetMesh());
	Waist = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Waist"));
	Waist->SetupAttachment(GetMesh());
	Leg = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Leg"));
	Leg->SetupAttachment(GetMesh());
	//Weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	//Weapon->SetupAttachment(GetMesh());

	//Animation 설정
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	
	
	//TODO: BeginPlay에서 로드할 것(현재는 확인용)
	//TODO: 임시 테이블 연동필요
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SKMesh(TEXT("/Game/AZ/Character/Mesh/FBone/SK_FBone"));
	GetMesh()->SetSkeletalMesh(SKMesh.Object);
	//static ConstructorHelpers::FClassFinder<UAnimInstance> ABPAnimInstance(TEXT("/Game/LHSCharacter/Animation/AnimInstance/ABP_COB"));
	//GetMesh()->SetAnimInstanceClass(ABPAnimInstance.Class);
	//CommonAnimInstanceClass = ABPAnimInstance.Class;
	static ConstructorHelpers::FClassFinder<UAnimInstance> ABPAnimInstance(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_WP00"));//ABP_WP00_
	GetMesh()->SetAnimInstanceClass(ABPAnimInstance.Class);
	WeaponAnimInstanceClass = ABPAnimInstance.Class;

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
	
	AZPlayerState = Cast<AAZPlayerState>(GetPlayerState());
	if(AZPlayerState)
	{
		PRINT_LOG("AZPlayerState SettingComplate");
	}
	//{
	//	(playerState->HeadItemID);//Table에서 파츠 경로로드
	//	(playerState->BodyItemID);
	//	(playerState->WaistItemID);
	//	(playerState->ArmItemID);
	//	(playerState->LegItemID);
	//	(playerState->WeaponItemID);
	//}

	//TODO: 데이터테이블로 처리하기(현재는 기본 파츠로)
	//Head->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(NULL, TEXT("/Game/LHSCharacter/Mesh/FHead/SK_FHead_???"), NULL, LOAD_None, NULL));//헤어와 헬멧 구분
	//Hair->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FHair/SK_FHair_121"), nullptr, LOAD_None, nullptr));
	//Face->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FFace/SK_FFace"), nullptr, LOAD_None, nullptr));
	//Body->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FBody/SK_FBody_500"), nullptr, LOAD_None, nullptr));
	//Arm->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FArm/SK_FArm_500"), nullptr, LOAD_None, nullptr));
	//Leg->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FLeg/SK_FLeg_500"), nullptr, LOAD_None, nullptr));
	//Waist->SetSkeletalMeshAsset(LoadObject<USkeletalMesh>(nullptr, TEXT("/Game/AZ/Character/Mesh/FWaist/SK_FWaist_500"), nullptr, LOAD_None, nullptr));
	//SetCombineSKMeshParts();
	
	//TODO: 무기 소켓에 붙혀서 사용해보기
	//등과 손에 각각
}

void AAZPlayer::BeginDestroy()
{
	Super::BeginDestroy();
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

//애니메이션을 변경하기 위한 함수
//Anim
//void ChangeAnim(){
//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
//AnimInstance->StopAllMontage(0.0f);
//
////static ConstructorHelpers::FClassFinder<UAnimInstance> ABPAnimInstance(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_CO1"));
//static ConstructorHelpers::FClassFinder<UAnimInstance> ABPAnimInstance(TEXT("/Game/AZ/Character/Animation/AnimInstance/ABP_WP0"));
//GetMesh()->SetAnimInstanceClass(ABPAnimInstance.Class);
//}
