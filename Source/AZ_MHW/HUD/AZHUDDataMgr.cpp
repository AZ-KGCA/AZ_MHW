// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUDDataMgr.h"

#include "AZ_MHW/Manager/AZResourceMgr.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "AZ_MHW/Util/AZUtility.h"

#include "Kismet/GameplayStatics.h"

void UAZHUDDataMgr::Init()
{
	widget_datas.Empty();

	// �⺻ ���� ����
	InitWidgetData(EUILayer::Scene, EUIName::None, false, false, TEXT(""));
	// ��������ȭ��
	InitWidgetData(EUILayer::Top, EUIName::AZWidget_Login, false, false, TEXT(""));
	// InOut
	InitWidgetData(EUILayer::Top, EUIName::AZWidget_BlackInOut, true, false, TEXT(""));

	widget_data_request_states.Empty();
}

void UAZHUDDataMgr::Reset()
{
	widget_data_request_states.Empty();
}

FAZWidgetData* UAZHUDDataMgr::GetWidgetData(EUIName ui_name_enum)
{
	return widget_datas.Find(ui_name_enum);
}

const FAZWidgetData* UAZHUDDataMgr::GetWidgetData(EUIName ui_name_enum) const
{
	return widget_datas.Find(ui_name_enum);
}

FAZWidgetData* UAZHUDDataMgr::GetSubWidgetData(ESubUIName ui_name_enum)
{
	return sub_widget_datas.Find(ui_name_enum);
}

EAZWidgetDataRequestState UAZHUDDataMgr::GetWidgetDataRequestState(EUIName ui_name)
{
	return widget_data_request_states.Contains(ui_name) ? widget_data_request_states[ui_name] : EAZWidgetDataRequestState::Completed;
}

bool UAZHUDDataMgr::RequestWidgetData(EUIName ui_name)
{
	EAZWidgetDataRequestState current_state = GetWidgetDataRequestState(ui_name);
	if (current_state == EAZWidgetDataRequestState::Completed)
	{
		//Data�� ������ ����, ������ ����.
		return true;
	}

	if (current_state == EAZWidgetDataRequestState::Requested)
	{
		// Data�� ��û ���� ����, ��ٸ�
		return false;
	}

	// FIXME (���� ������ Ȯ���ϱ�) Data�� ��û�Ѵ�.
	//if (auto* socketHolder = LHGameInstance->GetSocketHolder(ESocketHolderType::Gate))
	//{
	//	switch (uiName)
	//	{
	//	case EUIName::LHWidget_PKMain:
	//	{
	//		FCG_PK_TOTAL_INFO_SYN msg;
	//		socketHolder->SendPacket(&msg);
	//	}
	//	break;
	//	}

	//	if (WidgetDataRequestStates.Contains(uiName))
	//		WidgetDataRequestStates[uiName] = ELHWidgetDataRequestState::Requested;
	//}
	return false;
}

void UAZHUDDataMgr::OnSuccessWidgetDataRequest(EUIName ui_name)
{
	if (widget_data_request_states.Contains(ui_name) && widget_data_request_states[ui_name] == EAZWidgetDataRequestState::Requested)
	{
		widget_data_request_states[ui_name] = EAZWidgetDataRequestState::Completed;
	}
}

void UAZHUDDataMgr::OnFailedWidgetDataRequest(EUIName ui_name)
{
	if (widget_data_request_states.Contains(ui_name) && widget_data_request_states[ui_name] == EAZWidgetDataRequestState::Requested)
	{
		widget_data_request_states[ui_name] = EAZWidgetDataRequestState::None;
	}
}

void UAZHUDDataMgr::InitWidgetData(EUILayer ui_layer, EUIName ui_name, bool is_stay_in_viewport, bool disable_world_rendering, FString widget_path)
{
	FAZWidgetData ui_widget_data;
	ui_widget_data.layer = ui_layer;
	ui_widget_data.is_stay_in_viewport = is_stay_in_viewport;
	ui_widget_data.widget_id = (uint16)ui_name;
	ui_widget_data.widget_full_path = widget_path;
	ui_widget_data.disable_world_rendering = disable_world_rendering;
	widget_datas.Add(ui_name, ui_widget_data);
}

void UAZHUDDataMgr::InitWidgetDataRequestState(EUIName ui_name)
{
	widget_data_request_states.Emplace(ui_name, EAZWidgetDataRequestState::None);
}

UAZWidget* FAZWidgetData::GetWidget()
{
	UAZWidget* widget = widget_ptr.Get();
	return widget;
}

UAZWidget* FAZWidgetData::GetOrCreateWidget(bool& bGetWidget)
{
	UAZWidget* widget = widget_ptr.Get();
	if (widget == nullptr)
	{
		UClass* load_class = AZResourceHelper::LoadClassFast<UAZWidget>(widget_full_path);
		// FIXME Ȯ���ʿ�
		widget_ptr = CreateWidget<UAZWidget>(load_class->GetWorld()->GetGameInstance(), load_class);
		widget = widget_ptr.Get();
		bGetWidget = false;
	}
	else
	{
		bGetWidget = true;
	}

	if (widget == nullptr)
	{
		//�α� �߰�
	}
	return widget;
}

bool FAZWidgetData::IsWidgetValid() const
{
	return widget_ptr.IsValid();
}
