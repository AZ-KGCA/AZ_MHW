// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Widget/CharacterSelect/AZWidget_CharacterCreate.h"

#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Widget/System/AZButton.h"
#include "AZ_MHW/CommonSource/AZStruct.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/SocketHolder/AZSocketHolder.h"
#include "Kismet/GameplayStatics.h"

void UAZWidget_CharacterCreate::Init()
{
	Super::Init();

	AZ_PRINT_LOG_IF_FALSE(confirm_);
	confirm_->OnClicked.AddDynamic(this,&UAZWidget_CharacterCreate::OnConfirm);
	AZ_PRINT_LOG_IF_FALSE(back_);
	back_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnBack);
	AZ_PRINT_LOG_IF_FALSE(black_button_);
	black_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnBlack);
	AZ_PRINT_LOG_IF_FALSE(gray_button_);
	gray_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnGray);
	AZ_PRINT_LOG_IF_FALSE(red_button_);
	red_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnRed);
	AZ_PRINT_LOG_IF_FALSE(brown_button_);
	brown_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnBrown);
	AZ_PRINT_LOG_IF_FALSE(white_button_);
	white_button_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnWhite);


	AZ_PRINT_LOG_IF_FALSE(hair_button_0_);
	hair_button_0_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair0);
	AZ_PRINT_LOG_IF_FALSE(hair_button_1_);
	hair_button_1_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair1);
	AZ_PRINT_LOG_IF_FALSE(hair_button_2_);
	hair_button_2_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair2);
	AZ_PRINT_LOG_IF_FALSE(hair_button_3_);
	hair_button_3_->OnClicked.AddDynamic(this, &UAZWidget_CharacterCreate::OnHair3);
}

void UAZWidget_CharacterCreate::OnOpen(bool immediately)
{
	Super::OnOpen(immediately);
}

void UAZWidget_CharacterCreate::Update()
{
}

void UAZWidget_CharacterCreate::OnConfirm()
{
	FString id = nick_->GetText().ToString();
	FCStringAnsi::Strncpy(choose_character_info_.character_nick_, TCHAR_TO_ANSI(*id), sizeof(choose_character_info_.character_nick_) - 1);
	CS_PLAYER_CHARACTER_CREATE_REQ packet;
	packet.create_info = choose_character_info_;
	game_instance_->GetSocketHolder(ESocketHolderType::Game)->SendPacket(&packet, packet.packet_length);
}

void UAZWidget_CharacterCreate::OnBack()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/AZ/Map/Map_CharacterSelect"));
}

void UAZWidget_CharacterCreate::OnBlack()
{
	choose_character_info_.hair_color_id_ = 1;
}

void UAZWidget_CharacterCreate::OnGray()
{
	choose_character_info_.hair_color_id_ = 2;
}

void UAZWidget_CharacterCreate::OnRed()
{
	choose_character_info_.hair_color_id_ = 3;
}

void UAZWidget_CharacterCreate::OnBrown()
{
	choose_character_info_.hair_color_id_ = 4;
}

void UAZWidget_CharacterCreate::OnWhite()
{
	choose_character_info_.hair_color_id_ = 5;
}

void UAZWidget_CharacterCreate::OnHair0()
{
	choose_character_info_.hair_id_ = 1;
}

void UAZWidget_CharacterCreate::OnHair1()
{
	choose_character_info_.hair_id_ = 2;
}

void UAZWidget_CharacterCreate::OnHair2()
{
	choose_character_info_.hair_id_ = 3;
}

void UAZWidget_CharacterCreate::OnHair3()
{
	choose_character_info_.hair_id_ = 4;
}
