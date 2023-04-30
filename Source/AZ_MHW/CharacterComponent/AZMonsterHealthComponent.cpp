// Fill out your copyright notice in the Description page of Project Settings.

#include "AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"

UAZMonsterHealthComponent::UAZMonsterHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UAZMonsterHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AAZMonster* monster = Cast<AAZMonster>(GetOwner());
	if (monster)
	{
	}
}

void UAZMonsterHealthComponent::TakeDamage(AActor* damaged_actor, float damage_amount, const UDamageType* damage_type,
	AController* event_instigator, AActor* damage_causer)
{
}
