// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AZ_MHW/HUD/AZHUDDataMgr.h"
#include "AZ_MHW/HUD/AZHUDDefine.h"
#include "AZ_MHW/GameMode/AZGameMode.h"
#include "AZHUD.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API AAZHUD : public AHUD
{
	GENERATED_BODY()

	struct AZSceneData
	{
		AZSceneData() {}
		EUIName widget_name_enum;
		TArray<EUIName> child_widget_names;
		TSet<EUIName> bind_widget_set;

		int32 ChildCount() const
		{
			return child_widget_names.FilterByPredicate([this](const EUIName name) {
				return !bind_widget_set.Contains(name); }).Num();
		}
	};
	
public:
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;
	virtual void EndPlay(const EEndPlayReason::Type end_play_reson) override;
	virtual void Tick(float delta_seconds);
	virtual void OnTouchEmptySpace();
	virtual void InitHUD();

	AAZGameMode* GetGameMode();
	
	template <typename widget_type>
	widget_type* GetSubUI(ESubUIName widget_name_enum)
	{
		bool is_get_widget = false;
		FAZWidgetData* widget_data = GetSubWidgetData(widget_name_enum);
		if (widget_data == nullptr;)
		{
			return nullptr;
		}
		return Cast<widget_type>(widget_data->GetOrCreateWidget(is_get_widget));
	}

	template<typename widget_type>
	widget_type* GetUI(EUIName widget_name_enum)
	{
		FAZWidgetData* widget_data = GetWidgetData(widget_name_enum);
		if (widget_data && widget_data->IsWidgetValid())
		{
			return Cast<widget_type>(widget_data->GetWidget());
		}

		return nullptr;
	}


};
