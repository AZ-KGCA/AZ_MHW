// Copyright Team AZ. All Rights Reserved.

#include "AZMonsterPacketHandlerComponent_Client.h"
#include "AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h"
#include "AZ_MHW/Character/Monster/AZMonster_Client.h"

UAZMonsterPacketHandlerComponent_Client::UAZMonsterPacketHandlerComponent_Client()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UAZMonsterPacketHandlerComponent_Client::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Set owner as monster_client
	owner_ = Cast<AAZMonster_Client>(GetOwner());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterPacketHandlerComponent_Client] Invalid owner actor!"));
	}
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_TRANSFORM_CMD(FVector location, FRotator rotation, bool is_forced)
{
	if (is_forced)
	{
		owner_->SetActorLocation(location);
		owner_->SetActorRotation(rotation);
	}
	else
	{
		// TODO need interp
		owner_->SetActorLocation(location);
		owner_->SetActorRotation(rotation);
	}

	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][%d][Receive_SC_MONSTER_TRANSFORM_CMD] %s, Location: %s, Rotation %s"),
		owner_->object_serial_, is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *location.ToString(), *rotation.ToString());
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_BODY_STATE_CMD(
	FBossBodyPartDebuffState head_state, FBossBodyPartDebuffState back_state, FBossBodyPartDebuffState left_wing_state,
	FBossBodyPartDebuffState right_wing_state, FBossBodyPartDebuffState tail_state)
{
	// handle in mesh component
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Head, head_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Back, back_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::LeftWing, left_wing_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::RightWing, right_wing_state);
	owner_->mesh_component_->SetBodyState(EMonsterBodyPart::Tail, tail_state);

	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_BODY_STATE_CMD]"), owner_->object_serial_);
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_ENTER_COMBAT_CMD()
{
	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_ENTER_COMBAT_CMD]"), owner_->object_serial_);
	owner_->OnEnterCombat.Broadcast();
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_ACTION_START_CMD(FMonsterActionStateInfo action_info, float start_positon)
{
	owner_->SetActionStateInfo(action_info);
	
	// TODO Start position
	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_ACTION_START_CMD] Action name: %s, Montage section: %s, Start from: %f"),
		owner_->object_serial_, *action_info.animation_name.ToString(), *action_info.montage_section_name.ToString(), start_positon);
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type)
{
	switch (change_type)
	{
	case EMonsterBodyPartChangeType::Wound:
		{
			owner_->OnBodyPartWounded.Broadcast(body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Break:
		{
			owner_->OnBodyPartBroken.Broadcast(body_part);
			break;
		}
	case EMonsterBodyPartChangeType::Sever:
		{
			owner_->OnBodyPartSevered.Broadcast(body_part);
			break;
		}
	}

	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][#%d][Receive_SC_MONSTER_PART_CHANGE_CMD] %s %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(body_part), *UAZUtility::EnumToString(change_type));
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_HIT_CMD(FVector hit_position, int32 damage_amount)
{
	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][%d][Receive_SC_MONSTER_HIT_CMD] Damage dealt: %d, Position: %s"),
			owner_->object_serial_, damage_amount, *hit_position.ToString());
	//TODO UI 연결
}

void UAZMonsterPacketHandlerComponent_Client::Receive_SC_MONSTER_DIE_CMD()
{
	owner_->OnDeath.Broadcast();
	UE_LOG(AZMonster_Network, Log, TEXT("[UPacketFunction][Receive_SC_MONSTER_DIE_CMD]"));
}
