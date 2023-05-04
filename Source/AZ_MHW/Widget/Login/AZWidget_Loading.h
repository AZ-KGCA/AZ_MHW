// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_Loading.generated.h"

enum class EModelType : uint8
{
	Avatar,
	Monster,
	Npc,
	All
};

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Loading : public UAZWidget
{
	GENERATED_BODY()
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void NativeTick(const FGeometry& my_geometry, float inf_delta_time) override;

	virtual bool OnWidgetOpenAction(bool isVisibility = false) override;

	void SetLoadingModel(const class UModelData* model_data, int32 mapIndex);

	UFUNCTION() void OnPressedTouchScreen();
	UFUNCTION() void OnReleasedTouchScreen();
	UFUNCTION() void OnMoveTouchScreen(const FGeometry& my_geometry, const FPointerEvent& mouse_event);
};
