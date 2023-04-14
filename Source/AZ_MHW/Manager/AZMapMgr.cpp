// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Manager/AZMapMgr.h"

UAZMapMgr::UAZMapMgr()
{
}

UAZMapMgr::~UAZMapMgr()
{
}

void UAZMapMgr::Init()
{
}

void UAZMapMgr::LobbyEnterAck()
{
}

void UAZMapMgr::Tick(float delta_time)
{
}

void UAZMapMgr::OnGameMsg(FAZGameMsg* new_msg)
{
}

bool UAZMapMgr::IsTickable() const
{
	return !HasAnyFlags(EObjectFlags::RF_ClassDefaultObject);
}

bool UAZMapMgr::IsTickableInEditor() const
{
	return false;
}

bool UAZMapMgr::IsTickableWhenPaused() const
{
	return false;
}
