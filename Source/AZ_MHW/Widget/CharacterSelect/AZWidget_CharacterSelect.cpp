// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterSelect.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"

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
	//game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket()
}
