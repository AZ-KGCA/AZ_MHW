// Copyright Team AZ. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AZ_MHW/Widget/AZWidget.h"
#include "AZWidget_CharacterSelect.generated.h"

/**
 * 
 */
UCLASS()
class AZ_MHW_API UAZWidget_CharacterSelect : public UAZWidget
{
	GENERATED_BODY()

public:
	virtual void Init() override;
	virtual void OnOpen(bool immediately = false) override;
	virtual void Update() override;
public:
	void SetInfo();

public:
	UPROPERTY(meta = (BindWidget)) class UWidget* new_game_0_;
	UPROPERTY(meta = (BIndWidget)) class UWidget* new_game_1_;
	UPROPERTY(meta = (BindWidget)) class UWidget* new_game_2_;
	UPROPERTY(meta = (BindWidget)) class UWidget* character_slot_0_;
	UPROPERTY(meta = (BindWidget)) class UWidget* character_slot_1_;
	UPROPERTY(meta = (BindWidget)) class UWidget* character_slot_2_;

};
