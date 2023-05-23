// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/System/AZButton.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Widget/System/WidgetAction/AZWidgetAction.h"
#include <Components/ButtonSlot.h>
#include <Kismet/GameplayStatics.h>

#define LOCTEXT_NAMESPACE "UMG"

void SAZButton::Construct(const FArguments& in_args)
{
	SButton::Construct(SButton::FArguments()
		.OnClicked(in_args._OnClicked)
		.OnPressed(in_args._OnPressed)
		.OnReleased(in_args._OnReleased)
		.OnHovered(in_args._OnHovered)
		.OnUnhovered(in_args._OnUnhovered)
		.ButtonStyle(in_args._ButtonStyle)
		.ClickMethod(in_args._ClickMethod)
		.TouchMethod(in_args._TouchMethod)
		.PressMethod(in_args._PressMethod)
		.IsFocusable(in_args._IsFocusable));

	OnClickedInPosition = in_args._OnClickedInPosition;
	OnMouseMoveEvent = in_args._OnMouseMoveEvent;
}

FReply SAZButton::OnMouseButtonDown(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{
	pressed_screen_space_position_ = mouse_event.GetScreenSpacePosition();
	return SButton::OnMouseButtonDown(my_geometry, mouse_event);
}

FReply SAZButton::OnMouseButtonUp(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{
	FReply Reply = FReply::Unhandled();
	const EButtonClickMethod::Type InputClickMethod = GetClickMethodFromInputType(mouse_event);
	const bool bMustBePressed = InputClickMethod == EButtonClickMethod::DownAndUp || InputClickMethod == EButtonClickMethod::PreciseClick;
	const bool bMeetsPressedRequirements = (!bMustBePressed || (IsPressed() && bMustBePressed));

	if (bMeetsPressedRequirements && ((mouse_event.GetEffectingButton() == EKeys::LeftMouseButton || mouse_event.IsTouchEvent())))
	{
		Release();

		if (IsEnabled())
		{
			if (InputClickMethod == EButtonClickMethod::MouseDown)
			{
				// NOTE: If we're configured to click on mouse-down/precise-tap, then we never capture the mouse thus
				//       may never receive an OnMouseButtonUp() call.  We make sure that our bIsPressed
				//       state is reset by overriding OnMouseLeave().
			}
			else
			{
				bool bEventOverButton = IsHovered();

				if (!bEventOverButton && mouse_event.IsTouchEvent())
				{
					bEventOverButton = my_geometry.IsUnderLocation(mouse_event.GetScreenSpacePosition());
				}

				if (bEventOverButton)
				{
					// If we asked for a precise tap, all we need is for the user to have not moved their pointer very far.
					const bool bTriggerForTouchEvent = InputClickMethod == EButtonClickMethod::PreciseClick;

					// If we were asked to allow the button to be clicked on mouse up, regardless of whether the user
					// pressed the button down first, then we'll allow the click to proceed without an active capture
					const bool bTriggerForMouseEvent = (InputClickMethod == EButtonClickMethod::MouseUp || HasMouseCapture());

					/*if ((bTriggerForTouchEvent || bTriggerForMouseEvent))
					{
						Cast<UAZGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
						if (game_instance_->GetHUD())
						{
							AZGameInstance->GetHUD()->SetHandledTouchPosition(mouse_event);

							FVector2D position;
							if (AZGameInstance->GetHUD()->GetHandledTouchPosition(position))
								OnClickedInPosition.ExecuteIfBound(position);

							Reply = ExecuteOnClick();
						}
					}*/
				}
			}
		}

		//If the user of the button didn't handle this click, then the button's
		//default behavior handles it.
		if (Reply.IsEventHandled() == false)
		{
			Reply = FReply::Handled();
		}
	}

	//If the user hasn't requested a new mouse captor and the button still has mouse capture,
	//then the default behavior of the button is to release mouse capture.
	if (Reply.GetMouseCaptor().IsValid() == false && (DoesSharedInstanceExist() && HasMouseCapture()))
	{
		Reply.ReleaseMouseCapture();
	}

	Invalidate(EInvalidateWidget::Layout);

	return Reply;
}

FReply SAZButton::OnMouseMove(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{
	OnMouseMoveEvent.ExecuteIfBound(my_geometry, mouse_event);
	
	if (IsPressed() && IsPreciseTapOrClick(mouse_event) && FSlateApplication::Get().HasTraveledFarEnoughToTriggerDrag(mouse_event, pressed_screen_space_position_))
	{
		// Delta 값이 없으면 이동 안한걸로 판단한다.
		if (mouse_event.GetCursorDelta() != FVector2D::ZeroVector)
		{
			Release();
		}
	}

	return FReply::Unhandled();
}

FVector2D SAZButton::GetPressedScreenSpacePosition() const
{
	return pressed_screen_space_position_;
}

UAZButton::UAZButton()
{
	SButton::FArguments ButtonDefaults;
	WidgetStyle = *ButtonDefaults._ButtonStyle;

	ColorAndOpacity = FLinearColor::White;
	BackgroundColor = FLinearColor::White;

	ClickMethod = EButtonClickMethod::PreciseClick;
	TouchMethod = EButtonTouchMethod::PreciseTap;
	PressMethod = EButtonPressMethod::DownAndUp;

	IsFocusable = false;
	option_initialized_ = false;
	use_touch_mask_option_ = false;
	use_lock_option_ = false;
	touch_mask_settings_.mask_scale = FVector2D(1.f, 1.f);
	touch_mask_settings_.arrow_scale = 1.f;
	touch_mask_settings_.arrow_offset = 0.f;
	touch_mask_settings_.text_offset = 0.f;
	dimmed_ = false;

	ignore_click_effect_ = false;

	button_dimmed_instance_msg_id_ = 0;
}

void UAZButton::InitOption(ULHWidget* lhWidget)
{
	if (option_initialized_)
		return;

	option_initialized_ = true;

	if (use_touch_mask_option_)
	{
		// FIXME 마킹 기능 구현시 작업
		//lhWidget->AddLHButtonMaskMap(touch_mask_name_, this);
	}

	if (use_lock_option_)
	{
		// FIXME 락기능 구현시 작업
		//LockedWidget = lhWidget->GetWidgetFromName(LockedWidgetName);
		OnLockOptionDirty();
	}
}

void UAZButton::SetDimmed(bool dimmed)
{
	dimmed_ = dimmed;
	this->SetRenderOpacity(dimmed_ ? 0.3f : 1.f);
	this->OnLockOptionDirty();
}

void UAZButton::OnLockOptionDirty()
{
	// FIXME 락 구현시 구현
	/*if (use_lock_option == false || IsValid(locked_widget) == false || lock_option_setting.Num() <= 0)
		return;

	for (int32 i = 0; i < lock_option_setting.Num(); ++i)
	{
		lock_option_setting[i].locked = !AZGameInstance->QuestMgr->IsFinishedQuest(lock_option_setting[i].unlock_quest_index);

		switch (lock_option_setting[i].button_lock_option)
		{
		case ELHButtonLockOption::Dimmed:
		{
			locked_widget->SetRenderOpacity((AAZHUD::GetIgnoreWidgetLockOption() == false && lock_option_setting[i].locked) ? 0.3f : 1.f);
		}
		break;

		case ELHButtonLockOption::Invisible:
		{
			bool widgetIsButton = Cast<UButton>(locked_widget) != nullptr;
			locked_widget->SetVisibility((AAZHUD::GetIgnoreWidgetLockOption() == false && lock_option_setting[i].locked) ? ESlateVisibility::Hidden : (widgetIsButton ? ESlateVisibility::Visible : ESlateVisibility::SelfHitTestInvisible));
		}
		break;
		}

		locked_ = lock_option_setting[i].locked;

		if (lock_option_setting[i].locked)
			break;
	}*/
}

bool UAZButton::OnActivateTouchMask(int32 descStr)
{
	// FIXME 터치마스크 구현시 풀기
	/*if (!bUseTouchMaskOption)
		return false;

	if (ULHTouchMask::IsMaskable(this) == false)
		return false;

	if (auto* widget = LHGameInstance->GetHUD()->OpenUI<ULHWidget_TouchMask>(EUIName::LHWidget_TouchMask))
	{
		widget->SetLHButton(this);
		widget->SetParams(TouchMaskSettings);
		widget->SetText(LH_GetString(descStr));
	}*/
	return true;
}

bool UAZButton::OnActivateSearchTouchMask(const FLHTouchMaskSettings& settings, int32 descStr)
{
	/*if (ULHTouchMask::IsMaskable(this) == false)
		return false;

	if (auto* widget = LHGameInstance->GetHUD()->OpenUI<ULHWidget_TouchMask>(EUIName::LHWidget_TouchMask))
	{
		widget->SetLHButton(this);
		widget->SetParams(settings);
		widget->SetText(LH_GetString(descStr));
	}*/
	return false;
}

void UAZButton::OnClickedInPositionEvent(FVector2D position)
{
	if (ignore_click_effect_ == false)
	{
		if (!WidgetStyle.PressedSlateSound.GetResourceObject())
		{
			if (click_sound_ == nullptr)
			{
				// FIXME SoundManager 만들기
				/*if (AZ_SoundMgr())
				{
					click_sound_ = *AZ_SoundMgr()->SoundCue_Default_ButtonSound;
				}*/
			}
			if (click_sound_)
			{
				UGameplayStatics::PlaySound2D(GetWorld(), click_sound_);
			}
			// FIXME Effect 구현하기
			// AZGameInstance->GetHUD()->OnScreenEffect(position);
		}
	}

	OnClickedInPosition.ExecuteIfBound(position);
}

void UAZButton::OnMouseMove(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{
	OnMouseMoveEvent.ExecuteIfBound(my_geometry, mouse_event);
}

FReply UAZButton::GetSlateButtonMouseDown(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{

	return MyButton->OnMouseButtonDown(my_geometry, mouse_event);
}

FReply UAZButton::GetSlateButtonMouseUp(const FGeometry& my_geometry, const FPointerEvent& mouse_event)
{
	return MyButton->OnMouseButtonUp(my_geometry, mouse_event);
}

FVector2D UAZButton::GetPressedScreenSpacePosition() const
{
	return StaticCastSharedPtr<SAZButton>(MyButton)->GetPressedScreenSpacePosition();
}

void UAZButton::SetTouchMaskOption(const FName& mask_name, const FLHTouchMaskSettings& setting)
{
	use_touch_mask_option_ = true;
	touch_mask_name_ = mask_name;
	touch_mask_settings_ = setting;
}

void UAZButton::SetButtonDimmedInstanceMsgID(int32 button_dimmed_instance_msg_id)
{
	this->button_dimmed_instance_msg_id_ = button_dimmed_instance_msg_id;
}

void UAZButton::ForceClick()
{
	AZSlateHandleClicked();
}

TSharedRef<SWidget> UAZButton::RebuildWidget()
{
	MyButton = SNew(SAZButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, AZSlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, AZSlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, AZSlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		.OnClickedInPosition(BIND_UOBJECT_DELEGATE(FOnClickedInPositionEvent, OnClickedInPositionEvent))
		.OnMouseMoveEvent(BIND_UOBJECT_DELEGATE(FButtonMouseEvent, OnMouseMove));

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	if (press_widget_action_class_)
	{
		press_button_widget_action_ = Cast<UAZWidgetAction>(StaticConstructObject_Internal(press_widget_action_class_, this));
		AZ_PRINT_LOG_IF_FALSE(press_button_widget_action_, MyButton.ToSharedRef());
		press_button_widget_action_->SetTargetWidget(this);
	}

	return MyButton.ToSharedRef();
}

FReply UAZButton::AZSlateHandleClicked()
{
	// TODO need quest, show message
	/*if (ALHHUD::GetIgnoreWidgetLockOption() == false && bLocked)
	{
		// 퀘스트로 막힌거면 인스턴스 메세지
		int32 nQuestIndex = GetDimmedReasonQuestIndex();

		if (nQuestIndex != -1)
		{
			LHGameInstance->QuestMgr->ShowActiveWhenQuestClearMsg(nQuestIndex);
		}

		return FReply::Handled();
	}

	if (bDimmed && ButtonDimmedInstanceMsgID != 0)
	{
		LHGameInstance->GetHUD()->ShowInstantMsg(ButtonDimmedInstanceMsgID);
		return FReply::Handled();
	}*/

	return SlateHandleClicked();
}

void UAZButton::AZSlateHandlePressed()
{
	if (press_button_widget_action_)
	{
		press_button_widget_action_->BeginExecuteAction();
	}
	SlateHandlePressed();
}

void UAZButton::AZSlateHandleReleased()
{
	if (press_button_widget_action_)
	{
		press_button_widget_action_->BeginReleaseAction();
	}
	SlateHandleReleased();
}

int32 UAZButton::GetDimmedReasonQuestIndex()
{
	//TODO need quest
	return -1;
}
