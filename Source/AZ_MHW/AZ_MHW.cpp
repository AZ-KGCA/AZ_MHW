// Copyright Epic Games, Inc. All Rights Reserved.

#include "AZ_MHW.h"
#include "Modules/ModuleManager.h"

/** 핵심 게임 모듈*/
IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, AZ_MHW, "AZ_MHW" );

/** 디버그용 로그 카테고리 정의 */
DEFINE_LOG_CATEGORY(AZ_TEST);
DEFINE_LOG_CATEGORY(AZ_MOVE);
DEFINE_LOG_CATEGORY(AZ_ATTACK);
DEFINE_LOG_CATEGORY(AZ_DAMAGE);