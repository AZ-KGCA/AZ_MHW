// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <CoreMinimal.h>
#include "AZ_MHW.h"
#include <UObject/NoExportTypes.h>
#include "AZInputMgr.generated.h"
#pragma region Macro

#pragma endregion
#pragma region TypeDefine

#pragma endregion 
#pragma region ForwardDeclaration
class UInputAction;
class UInputMappingContext;
#pragma endregion
#pragma region Enum

#pragma endregion 
#pragma region Struct

#pragma endregion
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
	
#pragma region Inherited function
protected:
	/** */
	virtual void BeginDestroy() override;
#pragma endregion

public:
	/** !!따로 호출하지 마세오.(Controller SetupInputComponent에서 기본 호출)
	 * 
	 * 컨트롤러 제외한 기본 인풋매핑컨텍스트 사전등록 함수.
	 * 인풋액션바인딩 초기화구문이 있으니 컨트롤러의 액션바인딩보다 나중에 호출하지 마세요.
	 */
	void SetupDefaultBindAction(UInputComponent* PlayerInputComponent);
	
	UPROPERTY() TMap<FName, UInputMappingContext*> InputMappingContextMap;//데이터에셋 맵(접두사 제거 네임)
	UPROPERTY() TMap<FName, int32> InputMappingContextPriorityMap;//적용우선도 맵(접두사 제거 네임)
	UPROPERTY() TMap<FName, UInputAction*> InputActionMap;//인풋액션 맵(접두사 제거 네임)
	
	/** LocalPlayer의 Controller가 GetLocalPlayer()넘기면서 호출하기*/
	UFUNCTION() void AddInputMappingContext(const FName& IMCName, const  ULocalPlayer* LocalPlayer); 
	/** LocalPlayer의 Controller가 GetLocalPlayer()넘기면서 호출하기*/
	UFUNCTION() void RemoveInputMappingContext(const FName& IMCName, const  ULocalPlayer* LocalPlayer);
	/** LocalPlayer의 Controller가 GetLocalPlayer()넘기면서 호출하기*/
	UFUNCTION() void ClearInputMappingContext(const ULocalPlayer* LocalPlayer) const;
	/** */
	UFUNCTION() UInputAction* GetInputAction(const FName& Name);

private:
	UInputMappingContext* GetInputMappingContext(const FName& Name);
	int32* GetInputMappingContextPriority(const FName& Name);
	/** test용 함수 */
	void ActionTest();
};
