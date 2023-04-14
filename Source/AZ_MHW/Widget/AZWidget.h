// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Components/SlateWrapperTypes.h>
#include "AZWidget.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AZ_MHW_API UAZWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnWidgetClosed, bool);

public:
	//virtual void NativeConstruct() override;

	//virtual void SetVisiblity(ESlateVi)

	
};
