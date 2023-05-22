// Copyright Team AZ. All Rights Reserved.

#include "AZMonsterPacketHandlerComponent.h"

#include "AZMonsterHealthComponent.h"
#include "AZ_MHW/Character/Monster/AZMonster.h"
#include "AZ_MHW/GameInstance/CommonPacket.h"
#include "AZ_MHW/GameInstance/AZGameInstance.h"


// Sets default values for this component's properties
UAZMonsterPacketHandlerComponent::UAZMonsterPacketHandlerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UAZMonsterPacketHandlerComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Set owner as monster
	owner_ = Cast<AAZMonster>(GetOwner());
	if (!owner_.IsValid())
	{
		UE_LOG(AZMonster, Error, TEXT("[UAZMonsterPacketHandlerComponent] Invalid owner actor!"));
	}
}

// 플레이어 인게임 조인 패킷이 들어왔을때 몬스터를 스폰한 이후 호출되며 새로 들어온 클라이언트에 몬스터를 배치할 것을 명령한다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_SPAWN_CMD(UINT32 newly_joined_client_idx)
{
	SC_MONSTER_SPAWN_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_SPAWN_CMD);
	packet.packet_length = sizeof(packet);
	packet.monster_id = owner_->monster_id_;
	packet.location = owner_->GetActorLocation();
	packet.rotation = owner_->GetActorRotation();

	AZGameInstance->SendPacketFunc(newly_joined_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_SPAWN_CMD] ID: %d, Location: %s, Rotation %s"),
		owner_->object_serial_, packet.monster_id, *packet.location.ToString(), *packet.rotation.ToString());
}

// 1. 플레이어의 인게임 로딩이 끝난 뒤 몬스터의 최종 트랜스폼 설정을 명령할 때 새로 접속한 플레이어에게 전송된다 (is_forced = true)
// 2. 몬스터의 액션이 끝난 뒤 동기화를 위해 인게임 접속자 모두에게 전송된다 (is_forced = false)
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_TRANSFORM_CMD(bool is_forced, UINT32 newly_joined_client_idx)
{
	SC_MONSTER_TRANSFORM_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_TRANSFORM_CMD);
	packet.packet_length = sizeof(packet);
	packet.location = owner_->GetActorLocation();
	packet.rotation = owner_->GetActorRotation();
	packet.is_forced = is_forced;

	if (newly_joined_client_idx == -1)
		AZGameInstance->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	else
		AZGameInstance->SendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_TRANSFORM_CMD] %s, %s, Location: %s, Rotation %s"),
		owner_->object_serial_, newly_joined_client_idx == -1 ? TEXT("BROADCASTED") : TEXT(""), is_forced ? TEXT("FORCED") : TEXT("NOT FORCED"), *packet.location.ToString(), *packet.rotation.ToString());
}

// 플레이어의 인게임 로딩이 끝난 뒤 몬스터의 최종 메시 상태(상처, 부위파괴 등)의 설정을 위해 새로 접속한 플레이어에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_BODY_STATE_CMD(UINT32 newly_joined_client_idx)
{
	TMap<EMonsterBodyPart, FBossBodyPartState>* states = owner_->health_component_->GetBodyPartStates();

	SC_MONSTER_BODY_STATE_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_BODY_STATE_CMD);
	packet.packet_length = sizeof(packet);
	packet.head_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Head));
	packet.back_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Back));
	packet.left_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::LeftWing));
	packet.right_wing_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::RightWing));
	packet.tail_state = FBossBodyPartDebuffState(states->Find(EMonsterBodyPart::Tail));

	AZGameInstance->SendPacketFunc(newly_joined_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_BODY_STATE_CMD]"), owner_->object_serial_);
}

// 몬스터가 전투모드에 돌입할 때 노래 재생/UI등 업데이트를 위해 인게임 접속자 모두에게 전송된다
// 액션 정보로도 해결 가능 ? 굳이 없어도 되긴 할듯
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ENTER_COMBAT_CMD()
{
	SC_MONSTER_ENTER_COMBAT_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_ENTER_COMBAT_CMD);
	packet.packet_length = sizeof(packet);
	
	AZGameInstance->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_ENTER_COMBAT_CMD]"), owner_->object_serial_);
}

// 1. 몬스터의 액션 상태가 변경될 때 인게임 접속자 모두에게 전송된다
// 2. 플레이어의 첫 인게임 접속 시 현재 진행중인 액션 정보를 해당 플레이어에게 전송한다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_ACTION_START_CMD(float start_position, UINT32 newly_joined_client_idx)
{
	SC_MONSTER_ACTION_START_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_ACTION_START_CMD);
	packet.packet_length = sizeof(packet);
	packet.state_info = MonsterActionStateInfo(owner_->action_state_info_);
	packet.start_position = start_position;

	if (newly_joined_client_idx == -1)
		AZGameInstance->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	else
		AZGameInstance->SendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_ACTION_START_CMD] %s, Action name: %s, Montage section: %s, Start from: %f"),
		owner_->object_serial_, newly_joined_client_idx == -1 ? TEXT("BROADCASTED") : TEXT(""), *owner_->action_state_info_.animation_name.ToString(),
		*owner_->action_state_info_.montage_section_name.ToString(), start_position);
}

// 상처, 부위파괴, 절단 등 메시에 변화가 일어났을 때 인게임 접속자 모두에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_PART_CHANGE_CMD(EMonsterBodyPart body_part, EMonsterBodyPartChangeType change_type)
{
	SC_MONSTER_PART_CHANGE_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_BODY_STATE_CMD);
	packet.packet_length = sizeof(packet);
	packet.body_part = body_part;
	packet.change_type = change_type;

	AZGameInstance->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_PART_CHANGE_CMD], %s %s"),
		owner_->object_serial_, *UAZUtility::EnumToString(body_part), *UAZUtility::EnumToString(change_type));
}

// 플레이어의 공격이 몬스터에 맞았을 때 UI를 업데이트 하기 위해 공격한 플레이어에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_HIT_CMD(FVector hit_pos, int32 damage_amount, UINT32 attacker_client_idx)
{
	SC_MONSTER_HIT_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_HIT_CMD);
	packet.packet_length = sizeof(packet);
	packet.hit_position = hit_pos;
	packet.damage_amount = damage_amount;

	AZGameInstance->SendPacketFunc(attacker_client_idx, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_HIT_CMD], From: %d, Damage dealt: %d, Position: %s"),
		owner_->object_serial_, attacker_client_idx, damage_amount, *hit_pos.ToString());
}

// 몬스터가 사망했을 때 노래/UI등의 업데이트를 위해 모든 플레이어에게 전송된다
void UAZMonsterPacketHandlerComponent::Send_SC_MONSTER_DIE_CMD()
{
	SC_MONSTER_DIE_CMD packet(owner_->object_serial_);
	packet.packet_id = static_cast<int>(PACKET_ID::SC_MONSTER_DIE_CMD);
	packet.packet_length = sizeof(packet);
	
	AZGameInstance->BroadCastSendPacketFunc(-1, packet.packet_length, reinterpret_cast<char*>(&packet));
	UE_LOG(AZMonster_Network, Log, TEXT("[UAZMonsterPacketHandlerComponent][#%d][Send_SC_MONSTER_DIE_CMD]"), owner_->object_serial_);
}