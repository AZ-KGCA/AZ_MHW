// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AZTMovementComponent.generated.h"

enum class EMoveState : uint8 ;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class AZ_MHW_API UAZTMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAZTMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	bool IsMovable();
	//bool MoveBegin(EMoveType next_move_type, FVector next_pos, double angle);
	//EBTNodeResult MoveLoop();
	float GetMoveSpeed();

public:
	class AAZCharacter* owner_;
	
	EMoveState move_state_;
	
};
