// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/AZPlayer_Playable.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include <Components/CapsuleComponent.h>
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>

#include "GameInstance/AZGameInstance.h"
#include "Manager/AZInputMgr.h"
#include "AnimInstance/AZAnimInstance_Player.h"
#include "PlayerController/AZPlayerController_InGame.h"

#include "PlayerState/AZPlayerState.h"
//#include <InputActionValue.h>

AAZPlayer_Playable::AAZPlayer_Playable()
{
	PrimaryActorTick.bStartWithTickEnabled = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	SpringArmComp->SetupAttachment(GetCapsuleComponent());
	CameraComp->SetupAttachment(SpringArmComp);

	SpringArmComp->TargetArmLength = 400.f;
	SpringArmComp->bUsePawnControlRotation = true;
	CameraComp->bUsePawnControlRotation = false;
}

//TODO: CameraManager 만들어서 플레이어 캐릭터와 입력처리 완전히 분리
void AAZPlayer_Playable::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) 
	{
		EnhancedInputComponent->BindAction(AZGameInstance->input_mgr->GetInputAction("Look"), ETriggerEvent::Triggered, this, &AAZPlayer_Playable::ActionLook);
		//...
	}
}

void AAZPlayer_Playable::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	PlayablePlayerState = Cast<AAZPlayerState>(GetPlayerState());
	PlayablePlayerController = Cast<AAZPlayerController_InGame>(NewController);
}

void AAZPlayer_Playable::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	//유니티처럼 게임오브젝트를 하나 더만들어서 액터에 붙히고, 통신하게끔
	//시야각감지(플레이어 시선방향 45도 앞 상호작용 객체 쿼리)
	//거리감지(객체에 마다의 거리체크후 쿼리)

	PlayablePlayerState->CharacterState.CharacterDirection = GetRootComponent()->GetComponentRotation();
}


void AAZPlayer_Playable::ActionLook(const FInputActionValue& Value)
{
	//변경하지 않는 것에 대한 상수화
	const FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AAZPlayer_Playable::AnimNotify_OnUseItem()
{
	OnUseItem.Broadcast();
	
}

