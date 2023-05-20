// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget/AZWidget.h"
#include "AZWidget_Inventory.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_Inventory : public UAZWidget
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void OnClose(bool immediately = false) override;
	virtual void EditorOnly_Refresh() override;
	virtual void Update() override;
	virtual bool DifferenceBackButton() override;
	virtual void OnTouchEmptySpace() override;
	virtual void OnAlphaTimeFinished() override;
	virtual void RestoreFromForceHidden() override;
	virtual int32 GetSearchTouchMaskContentIndex(UWidget* widget, ETouchMaskSearchType search_type, int32 index) override;


};
