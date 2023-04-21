// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AZCharacter.h"

AAZCharacter::AAZCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void AAZCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AAZCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
