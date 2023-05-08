// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/PlayerState/AZPlayerState.h"

AAZPlayerState::AAZPlayerState()
{
	
}

void AAZPlayerState::BeginPlay()
{
	Super::BeginPlay();

	//데이터 가져오기
}

void AAZPlayerState::BeginDestroy()
{
	Super::BeginDestroy();
	
}

/**
	PlayerState 함수
	this->GetUniqueID();

	this->GetOwningController();

	this->GetPingInMilliseconds();

	this->GetPlayerName();

	this->GetPlayerController();

	this->GetPawn();
*/