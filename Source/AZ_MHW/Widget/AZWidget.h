// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AZ_MHW/HUD/AZHUDDefine.h"
//#include "AZ_MHW/Widget/System/WidgetAction/AZWidgetAction.h"
#include <Animation/WidgetAnimation.h>
#include <Components/SlateWrapperTypes.h>
#include "AZWidget.generated.h"

/**
 * 
 */

enum class EUIName : uint16;
UCLASS(Abstract)
class AZ_MHW_API UAZWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnWidgetClosed, bool);
public:
	//UAZWidget(const class FObjectInitializer& object_Initializer);
	//���� �������� ������ ����
	UAZWidget(FVTableHelper& helper);

	//  AddToViewport �� ȣ��ȴ�. NativeOnInitialized �� �޸�, Viewport �� Add �� ������ �Ҹ���!
	virtual void NativeConstruct() override;

	virtual void SetVisiblity(ESlateVisibility in_visiblilty);
	virtual void SetVisibilitySkipAction(ESlateVisibility in_visibility);

	class AAZGameMode* GetGameMode();
	class AAZHUD* GetHUD();
	// FIXME (���� �� �����ϱ�) // �ӽñ���
	APlayerController* GetPlayerController();
	//class ALHPlayerController* GetPlayerController();
	//class ALHPlayerController_InGame* GetPlayerController_InGame();

	// FIXME (���� �� �����ϱ�)
	//class AAZPlayer_Playable* GetPlayer();

	void SetWidgetNameEnum(EUIName name) { ui_name_ = name; }
	void StopAnimToOrigin(UWidgetAnimation* anim);
	EUIName GetWidgetNameEnum() { return ui_name_; }

	//AZHUD���� OpenUI�� ������ ������ ��� �ڵ� ȣ��.
	UFUNCTION(BlueprintCallable, Category = "AZ")
	virtual void Init();

	// UI�� Open�ɶ� ����
	virtual void OnOpen(bool immediately = false);
	virtual void OnExecuteOpen();

	// UI�� Open�ɶ�����
	virtual void OnClose(bool immediately = false);
	virtual void OnExecuteClose();

	// Open�ִϸ��̼� �������� �Ҹ�
	UFUNCTION()
	virtual void OnOpenAnimationFinished();
	virtual void ActivateContentsTutorial();

	// UI�� Close�� ������. CloseScene �� ���, �Ķ���Ϳ� ���� OnClose�� �ƴ� ForceHide�� ȣ�� ��
	UFUNCTION()
	virtual void OnCloseAnimationFinished();
	virtual void OnVisibilityClose();
	virtual void Destroy();
	void InitOption();

	void PlayAnimationLoop(UWidgetAnimation* anim, float play_back_speed = 1.0f);
	void OnOpenLoopAnim();

	UWidgetAnimation* GetWidgetAnimation(const FString& animation_name);

	virtual bool OnWidgetOpenAction(bool is_visibility = false);
	virtual bool OnWidgetCloseAction();

	FOnWidgetClosed widget_close_event;
	virtual void RestoreFromForceHidden();

	EUIState GetOpenUIState() { return open_ui_state_; }

	virtual void ForceHide();
	virtual void OnCoveredUI() {};

	bool GetIsForceHidden() { return is_force_hidden_; }

	void SetIsDisableBackBtnExit(bool is_disable_back_btn_exit) { is_disable_back_btn_exit_ = is_disable_back_btn_exit; }
	bool GetIsDisableBackBtnExit() { return is_disable_back_btn_exit_; }

	// AZHUD ���� OpenScene���� ������ ����, Scene ���ÿ��� ������ ��� �ڵ� ȣ��, �������� ���� ȣ��
	virtual void Update() {}
	virtual void Reset() {}

private:
	bool is_inited_ = false;
	EUIName ui_name_;
	EUIState open_ui_state_;
	ESlateVisibility visibility_beforce_force_hidden_;
	bool is_force_hidden_ = false;
	bool is_disable_back_btn_exit_ = false;
	int32 ui_layer_;
	float open_alpha_value_;
	FTimerHandle timer_handle_;
public:
	uint8 is_scene_ui : 1;

private:
	FOnWidgetClosed widget_close_event_;

protected:
	class UWidgetAnimation* open_ani;
	class UWidgetAnimation* close_ani;

protected:
	UPROPERTY(EditAnywhere, Category = AZWidget)
	bool is_widget_action_;

	UPROPERTY(EditAnywhere, Category = AZWidget)
	FString target_widget_name_;

	UPROPERTY(EditAnywhere, Category = AZWidget)
	TSubclassOf<class UAZWidgetAction> widget_action_class_;

	UPROPERTY() class UAZWidgetAction* widget_open_action_;

	UFUNCTION() void OpenAlphaTimeCallBack();
	virtual void OnAlphaTimeFinished();
};
