// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/HUD/AZHUD.h"

void AAZHUD::PostLoad()
{
    Super::PostLoad();
}

void AAZHUD::BeginPlay()
{
    Super::BeginPlay();
}

void AAZHUD::BeginDestroy()
{
    Super::BeginDestroy();
}

void AAZHUD::EndPlay(const EEndPlayReason::Type end_play_reason)
{
    Super::EndPlay(end_play_reason);
}

void AAZHUD::Tick(float delta_seconds)
{
    Super::Tick(delta_seconds);
}

void AAZHUD::OnTouchEmptySpace()
{
}

void AAZHUD::InitHUD()
{
}

AAZGameMode* AAZHUD::GetGameMode()
{
    return nullptr;
}
