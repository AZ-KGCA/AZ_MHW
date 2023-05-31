// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterSelect.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZGameCacheInfo.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UAZWidget_CharacterSelect::Init()
{
	Super::Init();
	AZ_PRINT_LOG_IF_FALSE(new_game_0_);
	new_game_0_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(new_game_1_);
	new_game_1_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(new_game_2_);
	new_game_2_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(character_slot_0_);
	character_slot_0_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(character_slot_1_);
	character_slot_1_->SetVisibility(ESlateVisibility::Collapsed);
	AZ_PRINT_LOG_IF_FALSE(character_slot_2_);
	character_slot_2_->SetVisibility(ESlateVisibility::Collapsed);

}

void UAZWidget_CharacterSelect::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
	CS_PLAYER_PLAYABLE_CHARACTER_DATA_REQ packet;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
}

void UAZWidget_CharacterSelect::Update()
{
	auto character_array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	if (character_array.Num() == 0)
	{
		new_game_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		new_game_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		new_game_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
	}
	if (character_array.Num() == 1)
	{
		character_slot_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_0_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[0].character_nick_)));
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_0);
		select_button_0_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_0);

		new_game_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		new_game_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
	}
	if (character_array.Num() == 2)
	{
		character_slot_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_0_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[0].character_nick_)));
		character_slot_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_1_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[1].character_nick_)));
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_0);
		select_button_0_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_0);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_1);
		select_button_1_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_1);

		new_game_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnNewGame);
	}
	if (character_array.Num() == 3)
	{
		character_slot_0_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_0_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[0].character_nick_)));
		character_slot_1_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_1_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[1].character_nick_)));
		character_slot_2_->SetVisibility(ESlateVisibility::HitTestInvisible);
		character_nick_name_2_->SetText(FText::FromString(ANSI_TO_TCHAR(character_array[2].character_nick_)));
		select_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_0);
		select_button_0_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_0);
		select_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_1);
		select_button_1_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_1);
		select_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterSelect::OnSelectCharacter_2);
		select_button_2_->OnHovered.AddDynamic(this, &UAZWidget_CharacterSelect::OnHoverCharacter_2);
	}

}

void UAZWidget_CharacterSelect::OnNewGame()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_CharacterCreate"));
}

void UAZWidget_CharacterSelect::OnSelectCharacter_0()
{
	auto array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	game_instance_->game_cache_info_->current_character_index_ = array[0].character_index_;
	array[0];
	//TODO
	// CharacterIndex 담아서 로그인 및 맵 이동(현수 부탁함)
}

void UAZWidget_CharacterSelect::OnSelectCharacter_1()
{
	auto array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	game_instance_->game_cache_info_->current_character_index_ = array[1].character_index_;
	array[1];
	//TODO
// CharacterIndex 담아서 로그인 및 맵 이동(현수 부탁함)
}

void UAZWidget_CharacterSelect::OnSelectCharacter_2()
{
	auto array = game_instance_->game_cache_info_->GetCurrentCharacterSimpleInfoArray();
	game_instance_->game_cache_info_->current_character_index_ = array[2].character_index_;
	array[2];
	//TODO
// CharacterIndex 담아서 로그인 및 맵 이동(현수 부탁함)
}

void UAZWidget_CharacterSelect::OnHoverCharacter_0()
{
}

void UAZWidget_CharacterSelect::OnHoverCharacter_1()
{
}

void UAZWidget_CharacterSelect::OnHoverCharacter_2()
{
}
