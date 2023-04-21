// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <UObject/NoExportTypes.h>
#include "AZInputMgr.generated.h"

class UInputAction;
class UInputMappingContext;

/**
 * InputMappingContext, InputAction 데이터들을 보유하는 매니저 클래스
 * BluePrint에서 사용해야 된다면, UCLASS에 지정자사용. 사용하기 쉽도록 TARRAY도 지원해야 한다. 
 */
UCLASS()
class AZ_MHW_API UAZInputMgr : public UObject
{
	GENERATED_BODY()
	
public:
	UAZInputMgr();

protected:
	/** */
	virtual void BeginDestroy() override;

public:
	/** !!따로 호출하지 마시오.(Controller SetupInputComponent에서 기본 호출)
	 * 
	 * 컨트롤러 제외한 기본 인풋매핑컨텍스트 사전등록 함수.
	 * 인풋액션바인딩 초기화구문이 있으니 컨트롤러의 액션바인딩보다 나중에 호출하지 마세요.
	 */
	void SetupDefaultBindAction(UInputComponent* PlayerInputComponent);
	/** test용 함수 */
	void ActionTest();

	UPROPERTY() TMap<FName, UInputMappingContext*> InputMappingContextMap;//데이터에셋 맵(접두사 제거 네임)
	UPROPERTY() TMap<FName, int32> InputMappingContextPriorityMap;//적용우선도 맵(접두사 제거 네임)
	UPROPERTY() TMap<FName, UInputAction*> InputActionMap;//인풋액션 맵(접두사 제거 네임)
};
/* #include "GameInstance/AZGameInstance.h"
	#include "Manager/AZInputMgr.h"
 */
//InputMappingContextMap.Find(접두사 제거 네임)의 역참조 ReturnType UInputMappingContext*
#define DEREF_IMCMAP(FindName) (*(AZGameInstance->input_mgr->InputMappingContextMap.Find(FindName)))
//InputMappingContextPriorityMap.Find(접두사 제거 네임)의 역참조 ReturnType int32
#define DEREF_IMCPMAP(FindName) (*(AZGameInstance->input_mgr->InputMappingContextPriorityMap.Find(FindName)))
//InputActionMap.Find(접두사 제거 네임)의 역참조 ReturnType UInputAction*
#define DEREF_IAMAP(FindName) (*(AZGameInstance->input_mgr->InputActionMap.Find(FindName)))