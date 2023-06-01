// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory_Container.h"

#include "AZ_MHW/Widget/Inventory/AZWidget_Inventory_Slot.h"
#include "Components/UniformGridSlot.h"
#include "AZ_MHW/HUD/AZHUD.h"

void UAZWidget_Inventory_Container::Init()
{
	Super::Init();
}

void UAZWidget_Inventory_Container::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	DisplaySampleSlots();
}

void UAZWidget_Inventory_Container::OnClose(bool immediately)
{
	Super::OnClose(immediately);
	item_slot_array_.Empty();
}

void UAZWidget_Inventory_Container::Update()
{
	Super::Update();
}

void UAZWidget_Inventory_Container::ForceHide()
{
	Super::ForceHide();
	SetVisibility(ESlateVisibility::Collapsed);
}

void UAZWidget_Inventory_Container::RestoreFromForceHidden()
{
	Super::RestoreFromForceHidden();
	SetVisibility(ESlateVisibility::Visible);
}

void UAZWidget_Inventory_Container::DisplaySampleSlots(int display_slot_count)
{
	int row_lenght = 10;
	for (int i = 0; i < 3; ++i)
	{
		UUniformGridPanel* grid_panel = GetUniformGridFromPanel(i);

		grid_panel->ClearChildren();
		int row = 0;
		int column = 0;
		for (int j = 0; j < display_slot_count; ++j)
		{
			UAZWidget_Inventory_Slot* slot = CreateWidget<UAZWidget_Inventory_Slot>(this, item_slot_);
			if (slot == nullptr)
			{
				continue;
			}

			// 기본 셋팅
			//...
			slot->Init();
			item_slot_array_.Add(slot);
			auto grid_slot = grid_panel->AddChildToUniformGrid(slot, row, column);
			grid_slot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Fill);
			grid_slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);

			column++;
			if (column >= row_lenght)
			{
				column = 0;
				++row;
			}
		}
	}
}

UUniformGridPanel* UAZWidget_Inventory_Container::GetUniformGridFromPanel(int32 panel_slot)
{
	UUniformGridPanel* grid_panel = nullptr;
	if (panel_slot == 0)
	{
		grid_panel = inventory_0_;
	}
	else if (panel_slot == 1)
	{
		grid_panel = inventory_1_;
	}
	else if (panel_slot == 2)
	{
		grid_panel = inventory_2_;
	}
	return grid_panel;
}
