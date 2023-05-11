// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include <Widgets/Input/SButton.h>
#include "AZ_MHW/Widget/AZWidgetData.h"
#include "AZButton.generated.h"

DECLARE_DELEGATE_OneParam(FOnClickedInPositionEvent, FVector2D);
DECLARE_DELEGATE_TwoParams(FButtonMouseEvent, const FGeometry&, const FPointerEvent&);

class AZ_MHW_API SAZButton : public SButton
{
public:
	SLATE_BEGIN_ARGS(SAZButton)
	{
	}
	SLATE_STYLE_ARGUMENT(FButtonStyle, ButtonStyle)
		SLATE_ARGUMENT(bool, IsFocusable)
		SLATE_EVENT(FOnClicked, OnClicked)
		SLATE_EVENT(FSimpleDelegate, OnPressed)
		SLATE_EVENT(FSimpleDelegate, OnReleased)
		SLATE_EVENT(FSimpleDelegate, OnHovered)
		SLATE_EVENT(FSimpleDelegate, OnUnhovered)
		SLATE_ARGUMENT(EButtonClickMethod::Type, ClickMethod)
		SLATE_ARGUMENT(EButtonTouchMethod::Type, TouchMethod)
		SLATE_ARGUMENT(EButtonPressMethod::Type, PressMethod)
		SLATE_EVENT(FOnClickedInPositionEvent, OnClickedInPosition)
		SLATE_EVENT(FButtonMouseEvent, OnMouseMoveEvent)
		SLATE_END_ARGS()

public:
	void Construct(const FArguments& in_args);
	virtual FReply OnMouseButtonDown(const FGeometry& my_geometry, const FPointerEvent& mouse_event) override;
	virtual FReply OnMouseButtonUp(const FGeometry& my_geometry, const FPointerEvent& mouse_event) override;
	virtual FReply OnMouseMove(const FGeometry& my_geometry, const FPointerEvent& mouse_event) override;

	FVector2D GetPressedScreenSpacePosition() const;

private:
	FOnClickedInPositionEvent OnClickedInPosition;
	FButtonMouseEvent OnMouseMoveEvent;
	FVector2D pressed_screen_space_position_;
};

UCLASS(Blueprintable, BlueprintType)
class AZ_MHW_API UAZButton : public UButton
{
	GENERATED_BODY()
	
public:
	UAZButton();

	void InitOption(class ULHWidget* lhWidget);
	void SetDimmed(bool isDimmed);
	void OnLockOptionDirty();
	bool OnActivateTouchMask(int32 descStr);
	bool OnActivateSearchTouchMask(const FLHTouchMaskSettings& settings, int32 descStr);
	bool IsLocked() const { return locked_; }
	bool IsDimmed() { return dimmed_; }

	void OnClickedInPositionEvent(FVector2D position);
	void OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	FReply GetSlateButtonMouseDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);
	FReply GetSlateButtonMouseUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	FVector2D GetPressedScreenSpacePosition() const;

	void SetTouchMaskOption(const FName& maskName, const FLHTouchMaskSettings& setting);

	void SetButtonDimmedInstanceMsgID(int32 buttonDimmedInstanceMsgID);
	void ForceClick();

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	FReply AZSlateHandleClicked();
	void AZSlateHandlePressed();
	void AZSlateHandleReleased();

	int32 GetDimmedReasonQuestIndex();

public:
	UPROPERTY(EditAnywhere, Category = "AZButton")
	TSubclassOf<class UAZWidgetAction> press_widget_action_class_;

	UPROPERTY() class UAZWidgetAction* press_button_widget_action_;

	UPROPERTY(EditAnywhere, Category = "AZButton")
	class USoundCue* click_sound_;

	FOnClickedInPositionEvent OnClickedInPosition;
	FButtonMouseEvent OnMouseMoveEvent;

	UPROPERTY(EditAnywhere, Category = "AZButton")
	int32 button_dimmed_instance_msg_id_;

	UPROPERTY(EditAnywhere, Category = "AZButton")
	uint8 ignore_click_effect_ : 1;

	UPROPERTY(EditAnywhere, Category = "AZ Button Lock Option", meta = (AllowPrivateAccess = "true"))
	uint8 use_lock_option_ : 1;

	UPROPERTY(EditAnywhere, Category = "AZ Button Lock Option", meta = (EditCondition = "use_lock_option", AllowPrivateAccess = "true"))
	FName locked_widget_name_;

	UPROPERTY(EditAnywhere, Category = "AZ Button Lock Option", meta = (EditCondition = "use_lock_option", AllowPrivateAccess = "true"))
	TArray< FAZLockOptionSettings> lock_option_setting_;

	UPROPERTY() UWidget* locked_widget;

private:
	uint8 option_initialized_ : 1;
	uint8 dimmed_ : 1;
	uint8 locked_ : 1;

	UPROPERTY(EditAnywhere, Category = "AZ Button TouchMask Option", meta = (AllowPrivateAccess = "true"))
	uint8 use_touch_mask_option_ : 1;

	UPROPERTY(EditAnywhere, Category = "AZ Button TouchMask Option", meta = (EditCondition = "use_touch_mask_option_", AllowPrivateAccess = "true"))
	FName touch_mask_name_;

	UPROPERTY(EditAnywhere, Category = "AZ Button TouchMask Option", meta = (EditCondition = "use_touch_mask_option_", AllowPrivateAccess = "true"))
	FLHTouchMaskSettings touch_mask_settings_;
};
