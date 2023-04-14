// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "AZ_MHW/HUD/AZHUDDefine.h"
#include "AZHUDDataMgr.generated.h"

UENUM()
enum class EAZWidgetDataRequestState
{
	None,
	Requested,
	Completed
};

USTRUCT(BlueprintType)
struct FAZWidgetData
{
	GENERATED_USTRUCT_BODY()

public:
	class UAZWidget* GetWidget();
	class UAZWidget* GetOrCreateWidget(bool& bGetWidget);
	bool IsWidgetValid() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZWidgetData")
	EUILayer layer = EUILayer::Scene;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZWidgetData")
	bool is_stay_in_viewport = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AZWidgetData")
	FString widget_full_path;

	uint16 widget_id;
	bool disable_world_rendering = false;

protected:
	//UPROPERTY()
	//TWeakObjectPtr<>
};

UCLASS()
class AZ_MHW_API UAZHUDDataMgr : public UObject
{
	GENERATED_BODY()
	
public:
	void Init();
	void Reset();
};
