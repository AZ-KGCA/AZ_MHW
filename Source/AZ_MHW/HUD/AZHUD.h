// Copyright Team AZ. All Rights Reserved.

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
	virtual void EndPlay(const EEndPlayReason::Type end_play_reason) override;
	virtual void Tick(float delta_seconds);
	virtual void OnTouchEmptySpace();
	virtual void InitHUD();

	AAZGameMode* GetGameMode();
	
	template <typename widget_type>
	widget_type* GetSubUI(ESubUIName widget_name_enum)
	{
		bool is_get_widget = false;
		FAZWidgetData* widget_data = GetSubWidgetData(widget_name_enum);
		if (widget_data == nullptr)
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

	template<typename widget_type>
	widget_type* OpenScene(EUIName widget_name_enum, bool is_need_restore = true)
	{
		AZSceneData* cur_scene_data = GetSceneData(widget_name_enum);
		if (cur_scene_data == nullptr)
		{
			return nullptr;
		}

		if (GetCurSceneNameEnum() == EUIName::AZWidget_InGame)
		{
			//FIXME Insert clear logic
		}

		cur_scene_name_enum = widget_name_enum;
		CheckOpenScene();

		EUIName prev_scene_name_enum = (scenes_stack.Num() > 0) ? scenes_stack.Top() : EUIName::None;
		AZSceneData* prev_scene_data = GetSceneData(prev_scene_name_enum);

		if (prev_scene_name_enum != EUIName::None && widget_name_enum != prev_scene_data->widget_name_enum)
		{
			CloseScene(prev_scene_data->widget_name_enum, false, false);
		}

		// If a stack exists in the scene you want to open, restore it.
		if (cur_scene_data->child_widget_names.Num() > 0)
		{
			if (is_need_restore == true)
			{
				for (int32 i = 0; i < cur_scene_data->child_widget_names.Num(); ++i)
				{
					FAZWidgetData* widget_data = GetWidgetData(cur_scene_data->child_widget_names[i]);

					if (widget_data == nullptr)
					{
						continue;
					}

					bool is_get_widget = false;
					UAZWidget* widget = widget_data->GetOrCreateWidget(is_get_widget);
					if (widget == nullptr)
					{
						continue;
					}

					widget->RestoreFromForceHidden();
					widget->Update();
				}

				RaiseOnTopFromStack(widget_name_enum);
			}
			else
			{
				while (cur_scene_data->child_widget_names.Num() > 1)
				{
					CloseUI(cur_scene_data->child_widget_names.Last());
				}
			}
		}
		else
		{
			scenes_stack.Push(cur_scene_name_enum);
			widget_type* scene_ui = OpenUI<widget_type>(cur_scene_data->widget_name_enum, true);
			if (UAZWidget* parent_ui = Cast<UAZWidget>(scene_ui))
			{
				parent_ui->is_scene_ui = true;
				parent_ui->bIsFocusable = true;
				parent_ui->SetFocus();
			}
		}

		for (int32 i = 0; i < OnExcuteAfterOpenScene.Num(); ++i)
		{
			OnExcuteAfterOpenScene[i].ExecuteIfBound();
		}

		OnExcuteAfterOpenScene.Empty();
		OnSceneOpened();

		return GetUI<widget_type>(cur_scene_data->widget_name_enum);
	}

	template<typename widget_type>
	widget_type* OpenUI(EUIName widget_name_enum, bool is_immediately = false, bool is_pre_scene = false)
	{
		FAZWidgetData* widget_data = GetWidgetData(widget_name_enum);
		if (widget_data == nullptr)
		{
			return nullptr;
		}

		widget_type* widget = nullptr;

		if ((widget_name_enum == EUIName::AZWidget_Waiting) && waiting_widget)
		{
			widget = (widget_type*)waiting_widget;
		}
		else
		{
			bool is_get_widget = false;
			widget = Cast<widget_type>(widget_data->GetOrCreateWidget(is_get_widget));
			if (widget == nullptr)
			{
				return nullptr;
			}
			widget->SetWidgetNameEnum(widget_name_enum);
			if (is_get_widget == false) // created
			{
				widget->Init();
			}
		}

		_OpenUI(widget, widget_data, is_immediately, is_pre_scene);

		return widget;
	}



	AZSceneData* GetSceneData(EUIName widget_name_enum);
	EUIName GetCurSceneNameEnum();
	FAZWidgetData* GetWidgetData(EUIName widget_name_enum);
	FAZWidgetData* GetWidgetData(EUIName widget_name_enum) const;
	FAZWidgetData* GetSubWidgetData(ESubUIName widget_name_enum);
	EUIName GetTopWidgetName();

	UFUNCTION(BlueprintCallable, Category = "AAZHUD") void CloseScene(bool isBackButton = false);
	void CloseScene(EUIName widget_name_enum, bool is_stack_delete = false, bool is_back_button = false);
	void CloseAllUI();
	void RaiseOnTopFromStack(EUIName ui_name);

	void _OpenUI(class UAZWidget* widget, FAZWidgetData* widget_data, bool is_immediately = false, bool is_pre_scene = false);

	bool CloseUI(EUIName ui_name, bool is_immediately = false, bool is_remove_stack = true, bool is_check_next_show_ui = false);
	bool CloseUI(class UAZWidget* widget, bool is_immediately = false, bool is_remove_stack = true, bool is_check_next_show_ui = false);

	void UpdateWorldRender();
	void SetWorldRender(bool value);

	DECLARE_DELEGATE(FShowAfterCloseUI)
	TArray<FShowAfterCloseUI> OnShowUIListAfterCloseUI;

	DECLARE_DELEGATE(FExcuteAfterOpenScene)
	TArray<FExcuteAfterOpenScene> OnExcuteAfterOpenScene;

public:
	void OnFadeInOut(const float in_time, const float out_time);

protected:
	virtual void CheckOpenScene() {}

	virtual void OnSceneOpened();
	virtual void OnSceneClosed();

protected:
	TMap<EUIName, AZSceneData> scene_datas;
	EUIName cur_scene_name_enum;
	TArray<EUIName> scenes_stack;
	UPROPERTY() class UAZWidget_Waiting* waiting_widget;
	UPROPERTY() int32 hide_ui_flag = 0;
};
