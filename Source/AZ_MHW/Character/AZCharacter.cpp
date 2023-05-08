// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Character/AZCharacter.h"

AAZCharacter::AAZCharacter()
{
	//PrimaryActorTick.bCanEverTick = true;
}

void AAZCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AAZCharacter::SetupPlayerInputComponent(UInputComponent* player_input_component)
{
	Super::SetupPlayerInputComponent(player_input_component);

}
