// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/Widget/Login/AZWidget_Loading.h"

#include "Components/Image.h"
#include "ShaderPipelineCache.h"


void UAZWidget_Loading::Init()
{
	Super::Init();

	// FIXME need build
	/*text_animation = GetWidgetAnimation(TEXT("Text_Ani"));
	fade_out_animation = GetWidgetAnimation(TEXT("Fade_Out"));
	loading_bg_image = GetOwnWidget<UImage>(TEXT("Image_0"));

	c_touch_screen->OnPressed.Clear();
	c_touch_screen->OnPressed.AddDynamic(this, &UAZWidget_Loading::OnPressedTouchScreen);
	c_touch_screen->OnReleased.Clear();
	c_touch_screen->OnReleased.AddDynamic(this, &UAZWidget_Loading::OnReleasedTouchScreen);
	c_touch_screen->OnMouseMoveEvent.Unbind();
	c_touch_screen->OnMouseMoveEvent.BindUObject(this, &UAZWidget_Loading::OnMoveTouchScreen);*/
}

void UAZWidget_Loading::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	// FIXME need build
	/*cached_ratio = 0.0f;

	FShaderPipelineCache::SetBatchMode(FShaderPipelineCache::BatchMode::Fast);*/
}

void UAZWidget_Loading::NativeTick(const FGeometry& my_geometry, float in_delta_time)
{
	Super::NativeTick(my_geometry, in_delta_time);

	//FIXME need build
	/*if (c_touch_screen->IsPressed() == false)
	{
		OnMoveTouchScreen(my_geometry, FPointerEvent());
	}

	auto canvas_rect = c_canvas->GetCachedGeometry().GetLayoutBoundingRect().GetSize();
	if (FMath::IsNearlyZero(canvas_rect.X) || FMath::IsNearlyZero())*/
}

bool UAZWidget_Loading::OnWidgetOpenAction(bool isVisibility)
{
	return false;
}

void UAZWidget_Loading::SetLoadingModel(const UModelData* model_data, int32 mapIndex)
{
	// FIXME animation tooltip, avartar apper
	/*PlayAnimation(fade_out_animation);
	PlayAnimationLoop(text_animation);

	loading_bg_image->SetVisibility(ESlateVisibility::Collapsed);
	c_image_loading_actor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	c_desc_widget_switcher->SetActiveWidgetIndex(Avatar);
	SetModelText(modle_data);*/

	// FIXME loading animation tooltip, avartar apper
	//int32 level = 1;
	//if (GetPlayer() && GetPlayer()->CharacterParam)
	//	level = GetPlayer()->CharacterParam->GetLevel();

	//TArray<int32> tipStringCodeList;
	//for (auto* tipRec : GetTable(ULoadingTable_Client)->m_TipInfoSheet->arrayTable)
	//{
	//	if (auto* loadingInfo = GetTable(ULoadingTable_Client)->GetLoadingInfoFromMapIndex(mapIdx))
	//	{
	//		if (loadingInfo->TipGroupID == tipRec->TipGroup && level >= tipRec->LevelLimit)
	//			tipStringCodeList.Add(tipRec->StringCode);
	//	}
	//}



	//if (tipStringCodeList.Num() > 0 && IsValid(C_Text_Tip))
	//{
	//	int32 selected = FMath::Rand() % tipStringCodeList.Num();
	//	C_Text_Tip->SetTextTableID(tipStringCodeList[selected]);
	//}
}

void UAZWidget_Loading::OnPressedTouchScreen()
{
}

void UAZWidget_Loading::OnReleasedTouchScreen()
{
}

void UAZWidget_Loading::OnMoveTouchScreen(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{
}
