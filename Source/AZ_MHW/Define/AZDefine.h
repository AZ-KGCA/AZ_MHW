// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

/** Unreal Engine 로우레벨 헤더*/
//#include <CoreFwd.h>//전방선언만
#include <CoreMinimal.h>//필수만
//#include <Core.h>//모든 헤더(사용하지 말 것)

//C# 리플레션, 가비지컬렉터, 프로퍼티 등
//#include <CoreUObject.h>

/** Unreal Engine 전방선언 헤더*/
//#include <EngineFWD.h>

/** Unreal Engine 핵심 헤더만 */
//#include <EngineMinimal.h>

/** Unreal Engine 모든 헤더 */
//#include <Engine.h>(사용하지 말 것)
//#include <UnrealEd.h>(사용하지 말 것)

#pragma region MACRO
//빨간 에러 표시줄 거슬려서 수정 
#undef DECLARE_LOG_CATEGORY_EXTERN
#define DECLARE_LOG_CATEGORY_EXTERN(CategoryName, DefaultVerbosity, CompileTimeVerbosity) \
		struct FLogCategory##CategoryName : public FLogCategory<ELogVerbosity::DefaultVerbosity, ELogVerbosity::CompileTimeVerbosity> \
		{ \
			FORCEINLINE FLogCategory##CategoryName() : FLogCategory(TEXT(#CategoryName)) {} \
		};\
		extern FLogCategory##CategoryName CategoryName;

//게임화면 출력용 디버깅 로그
#define DISPLAY_LOG(fmt,...) \
		if(GEngine) GEngine->AddOnScreenDebugMessage (-1, 5.f, FColor::Red, FString::Printf(TEXT(fmt), __VA_ARGS__));

/** 디버그용 전역 로그구조체 선언
* ProjectET.cpp파일에 선언됨
*/
DECLARE_LOG_CATEGORY_EXTERN(AZ_TEST, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ_MOVE, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ_ATTACK, Log, All)
DECLARE_LOG_CATEGORY_EXTERN(AZ_DAMAGE, Log, All)

/** 귀찮은 테스트 로그 이걸로*/
#define PRINT_LOG(str)		UE_LOG(AZ_TEST, Warning, TEXT(str))
#define PRINT_FUNCTION()	UE_LOG(AZ_TEST, Warning, TEXT("%s"), __FUNCTIONW__)

/** 열거형 이름 문자열반환 매크로
 * 이것만 없길레 새로 정의함. 
 * #include <Misc/AssertionMacros.h> UEAsserts_Private 필요
 */
#define GET_ENUMERATOR_NAME_STRING_CHECKED(EnumName, EnumeratorName) \
	((void)sizeof(UEAsserts_Private::GetMemberNameCheckedJunk(EnumName::EnumeratorName)), TEXT(#EnumeratorName))

#pragma endregion
#pragma region TypeDefine

#pragma endregion
#pragma region EnumClass_Enum
/** Enum Table Auto Generated. */
//#include "AZEnumTable.h"

#pragma endregion
#pragma region ForWardDeclaration

#pragma endregion
#pragma region Struct

#pragma endregion



//일반적인 헤더 C++ 순서 묶음
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration

#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion

//언리얼 클래스 묶음
#pragma region Inherited function

#pragma endregion


#pragma region //기능별로 묶기

#pragma endregion
#pragma region //변수, 프로퍼티, 옵저버블

#pragma endregion
#pragma region //함수, 델리게이트, 이벤트

#pragma endregion