#pragma once

#include "AZHUDDefine.generated.h"

UENUM(BlueprintType)
enum class EUIMsgBoxType : uint8
{
	None = 0,
	Basic
};

UENUM(BlueprintType)
enum class EUIMsgBoxBtnType : uint8
{
	OkOrCancel,
	Confirm,
	LeftOrRight,
	ScreenButton
};

UENUM(BlueprintType)
enum class EUIValueStyle : uint8
{
	None = 0,
	Percent,
	Second
};

USTRUCT(BlueprintType)
struct FMsgBoxInfo
{
	GENERATED_USTRUCT_BODY()

	EUIMsgBoxType msg_box_type = EUIMsgBoxType::Basic;
	FString desc;
	EUIMsgBoxBtnType btn_type = EUIMsgBoxBtnType::Confirm;
	UObject* owner = nullptr;
	FName left_tap_function_name;
	FName right_tap_function_name;
	FName exit_function_name;
	FString left_btn_text;
	FString right_btn_text;
	bool is_add_wait_msg_box_stack = true;
	bool is_disable_back_btn_exit = false;
};

UENUM()
enum class EUIHideState : int32
{
	AllHide = 1 << 0, // 1
	AllWidgetHide = 1 << 1, // 2,
	LeftTopHide = 1 << 2, // 4,
	LeftDownHide = 1 << 3, // 8,
	RightTopHide = 1 << 4, // 16
	RightDownHide = 1 << 5, // 32	
};

UENUM(BlueprintType)
enum class EUIState : uint8
{
	None,
	Opened,
	Closed,
	Max
};

UENUM(BlueprintType)
enum class EUILayer : uint8
{
	None,
	World,
	Scene,
	Popup,
	WaitingWidget,
	Top,
	Max
};

enum class ESubUIName : uint16
{
	None = 0,
	Max
};

enum class EUIName : uint16
{
	None = 0,
	AZWidget_Splash,
	AZWidget_Black,
	AZWidget_BlackInOut,
	AZWidget_Login,
	AZWidget_Menu,
	AZWidget_Loading,
	AZWidget_Waiting,
	AZWidget_InGame,
	AZWidget_Scene_Launcher,
	AZWidget_Fade,
};