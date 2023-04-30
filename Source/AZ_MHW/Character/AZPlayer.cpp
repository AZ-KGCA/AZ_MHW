// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Character/AZPlayer.h"
#include "AZ_MHW/CommonSource/AZEnum.h"

AAZPlayer::AAZPlayer()
{
	team_id_ = uint8(EObjectType::Player);
}
