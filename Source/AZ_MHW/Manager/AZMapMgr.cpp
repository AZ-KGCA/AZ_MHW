// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/Manager/AZMapMgr.h"

#include "AZ_MHW/GameInstance/AZGameInstance.h"
#include "AZ_MHW/Manager/AZTableMgr.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/CommonSource/AZLog.h"
#include "AZ_MHW/Map/AZMapBase.h"
#include "AZ_MHW/Login/AZLoginMgr.h"
#include "AZ_MHW/Util/AZUtility.h"
#include "AZ_MHW/Manager/AZResourceMgr.h"
#include "AZ_MHW/GameMode/AZGameMode_InGame.h"
#include "AZ_MHW/Map/AZWorldMap.h"
#include "AZ_MHW/Widget/InGame/AZWidget_InGame.h"
#include "AZ_MHW/Widget/Login/AZWidget_Loading.h"
#include "AZ_MHW/HUD/AZHUD.h"
#include "AZ_MHW/Map/AZMapBase.h"
#include "AZ_MHW/CommonSource/AZGameMsg.h"
#include "AZ_MHW/Environment/AZActor_LoadingScene.h"
#include "AZ_MHW/Widget/Login/AZWidget_Loading.h"
#include "AZ_MHW/CommonSource/AZEnum.h"
#include "AZ_MHW/CommonSource/Table/ModelData.h"
#include "AZ_MHW/CommonSource/Table/LoadingData.h"
#include "AZ_MHW/CommonSource/Table/MapData.h"
#include "AZ_MHW/Environment/AZActor_LoadingScene.h"
#include "Components/SkyLightComponent.h"
#include "Components/LightComponent.h"
#include "ShaderPipelineCache.h"
#include "Math/Box.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/UObjectBaseUtility.h"
#include "Engine/WorldComposition.h"
#include "Engine/LevelStreaming.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Engine/PostProcessVolume.h"
#include "GameFramework/Character.h"

const FName TAG_LoadingCheck(TEXT("AZ_LoadingCheck"));

UAZMapMgr::UAZMapMgr()
{
	loop_timer_ = 0.25;
	is_map_entered_ = false;
	is_portal_moving_ = false;
	enter_pos_ = FVector::ZeroVector;
	is_loading_ = false;
	close_loading_scene_on_finish_ = false;
	flush_streaming_ = false;
	ready_server_sync_streaming_ = false;
	use_teleport_after_sync_streaming_ = false;
	current_streaming_state_ = EStreamingState::None;
	reserved_force_kick_ = EForceKick::Max;
	same_map_enter_ready_ = false;
	saved_auto_move_data_.Reset();
	loading_map_index_ = 0;
	disconnected_caching_map_index_ = 0;

}

UAZMapMgr::~UAZMapMgr()
{
	// FIXME �Ʒ��� �� �ʱ�ȭ
}

void UAZMapMgr::Init()
{
	same_map_enter_ready_ = false;
}

void UAZMapMgr::LobbyEnterAck()
{
	MapDestroy();
}

void UAZMapMgr::Tick(float delta_time)
{
	if (GetDefault<UAZMapMgr>(UAZMapMgr::StaticClass()) == this)
	{
		return;
	}

	UpdateStreaming();

}

void UAZMapMgr::OnGameMsg(FAZGameMsg* new_msg)
{
	if (IsValid(current_map_))
	{
		current_map_->OnGameMsg(new_msg);
	}
}

bool UAZMapMgr::IsTickable() const
{
	return !HasAnyFlags(EObjectFlags::RF_ClassDefaultObject);
}

bool UAZMapMgr::IsTickableInEditor() const
{
	return false;
}

bool UAZMapMgr::IsTickableWhenPaused() const
{
	return false;
}

void UAZMapMgr::MapEnter(uint32 map_index, FVector enter_pos, float enter_angle)
{
	if (reserved_force_kick_ != EForceKick::Max)
	{
		AZGameInstance->login_mgr->OnForceKicked(reserved_force_kick_);
		reserved_force_kick_ = EForceKick::Max;
		return;
	}


	// ���� �ʿ��� ����
	OnMapExit();

	enter_pos_ = enter_pos;
	enter_angle_ = enter_angle;

	const UMapData* before_map_data = GetMapData();
	if (current_map_)
	{
		current_map_->OnMapExit();
	}

	current_map_ = CreateMap(map_index);
	current_map_->OnCreate(GetTableByIndex(UMapData, map_index));


	dungeon_info_ = nullptr;
	if (Cast<UAZDungeon>(current_map_))
	{
		dungeon_info_ = Cast<UAZDungeon>(current_map_);
	}

	const UMapData* map_data = GetMapData();

	// FIXME ������ ��ġ�ϱ�(���߿� �ֱ�)
	//{

	//}

	bool is_open_map = true;
	if (before_map_data && map_data->map_name.Compare(before_map_data->map_name) == 0)
	{
		is_open_map = false;
	}

	// ���� �ε��ϰ� Player�� ��ġ��Ŵ
	if (map_data != nullptr)
	{
		if (is_open_map == true)
		{
			AZResourceHelper::ClearReference(EAZResourceCacheType::MapOnly);

			if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(AZGameInstance->GetGameMode()))
			{
				game_mode->GetWorldMapHandler()->ClearWorldMapInfo();
			}

			FString map_path = FString::Printf(TEXT("/Game/Maps/%s"), *map_data->map_name);

			OpenLoadingScene(map_index);

			GEngine->ForceGarbageCollection(true);

			bool use_seamless_loading = true;
			if (use_seamless_loading == true)
			{
				AZGameInstance->GetWorld()->SeamlessTravel(map_path);
			}
			else
			{
				FName map_name(*map_path);
				UGameplayStatics::OpenLevel(AZGameInstance->GetWorld(), map_name);
			}

			post_load_map_delegate_handle_ = FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UAZMapMgr::PostLoadMapFunction);

			/*if (AZGameInstance != nullptr)
			{
				AZGameInstance->voidSummonerMgr->ResetSpawnnedInfo();
			}

			if (AZGameInstance->SoundAlarmMgr)
			{
				AZGameInstance->SoundAlarmMgr->ClearAllTimers();
			}

			if (AZGameInstance->InputMgr)
			{
				AZGameInstance->InputMgr->ClearBlockFlag();
			}*/
		}

		/*if (AZGameInstance && AZGameInstance->IsRealmWar())
		{
			if ((EMapType)map_data->map_type != EMapType::RealmWar)
			{
				AZGameInstance->RealmWarMgr->EndRealmwar();
			}
		}*/

		if (is_open_map == false) // �������� ������ ����
		{
			bool immediately_same_map_enter_proc = true;
			// ���� �κ������ ���ƿ�
			EMapType map_type = UAZUtility::StringToEnum<EMapType>(map_data->map_type);
			if (EMapType::Lobby == static_cast<EMapType>(map_type))
			{
				//if (LHGameInstance->QuestMgr->IsReservedBeginChapter())
				//{
				//	if (ALHGameMode_InGame* gm = Cast<ALHGameMode_InGame>(LHGameInstance->GetGameMode()))
				//	{
				//		if (ALHPlayer_Playable* player = LHGameInstance->GetPlayer())
				//		{
				//			if (player->GetNpcTalkComponent()->IsTalk())
				//				player->EndNpcTalk();
				//		}
				//		bImmediatelySameMapEnterProc = false;
				//		gm->GetStateMachine()->ChangeState(EGameState::BeginChapter);

				//		auto func = [this, enterPos, enterAngle]()->void			// ���� ���� ���߿� ���� ������ ��ġ �̵� 
				//		{
				//			this->SameMapEnterProc(enterPos, enterAngle);
				//		};
				//		CallBackQuestWarp = func;
				//	}
				//}
				//else
				//{
				//	if (ULHWidget_BlackInOut* ui = LHGameInstance->GetHUD()->OpenUI<ULHWidget_BlackInOut>(EUIName::LHWidget_BlackInOut))
				//	{
				//		auto func = [ui, this, enterPos, enterAngle]()->void
				//		{
				//			ui->PlayExit();
				//			this->SameMapEnterProc(enterPos, enterAngle);
				//		};
				//		bImmediatelySameMapEnterProc = false;
				//		ui->SetCallBackFunc(func);
				//	}
				//}

				//if (ULHWidget_InGame* ui = LHGameInstance->GetHUD()->GetUI<ULHWidget_InGame>(EUIName::LHWidget_InGame))
				//{
				//	ui->ChangeExitSwitcher(false);
				//}

				//if (ALHGameMode_InGame* gm = Cast<ALHGameMode_InGame>(LHGameInstance->GetGameMode()))
				//{
				//	gm->ActiveEnvironmentReset();
				//}
			}
			else if (EMapType::Dungeon == map_type)
			{
				/*if (UAZWidget_InGame* ui = AZGameInstance->GetHUD()->GetUI<UAZWidget_InGame>(EUIName::AZWidget_InGame))
				{
					ui->ChangeExitSwitcher(true);
				}*/
			}

			if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(AZGameInstance->GetGameMode()))
			{
				//game_mode->ResetAreaPortalAndWarp();
				game_mode->RemoveAllAppearObjects();
			}
		}
	}
}

void UAZMapMgr::SameMapEnterPorc(FVector enter_pos, float enter_angle)
{
	same_map_enter_ready_ = true;

	// FIXME ���ս� Ǯ��
	/*AAZPlayer_Playable* player = AZGameInstance->GetPlayer();
	if (player != nullptr)
	{
		// ���������� ���� ���̶�� ��ġ �̵�
		player->OnWarp(enter_pos, enter_angle);

		// FIXME �̵� �� Ż �� ������ (���������� ������ ������ ó��)
	}*/

	// FIXME ���θ޴� ���� ����
	
	// FIXME ����Ʈ ui ���ֱ�

	CloseLoadingScene();
	SendMapEnter();
}

void UAZMapMgr::PostLoadMapFunction(UWorld* world)
{
	AZ_PRINT_LOG_IF_FALSE(world);

	pause_streaming_ = false;
	character_only_streaming_ = false;
	current_streaming_state_ = world->WorldComposition ? Waiting : None;

	TArray<AActor*> actors;
	// FIXME ���ս� Ǯ��
	/*UGameplayStatics::GetAllActorsOfClass(world, AAZPlayer_Playable::StaticClass(), actors);
	if (actors.Num() > 0)
	{
		if (AAZPlayer_Playable* start = Cast<AAZPlayer_Playable>(actors[0]))
		{
			start->SetActorServerLocation(GetEnterPos());
			if (auto* movement = start->GetAZCharacterMovement_Sync())
			{
				movement->SetIgnoreSync(true);
			}
		}
	}*/

	if (world->WorldComposition)
	{
		//FIXME �÷����ο��� ������ �Ѱ����� ����
		/*if (world->bDisableUpdateStreamingState == false)
		{
			world->bDisableUpdateStreamingState = true;
		}*/

		if (world->WorldComposition->GetOutermost()->HasAnyPackageFlags(PKG_PlayInEditor))
		{
			world->WorldComposition->ReinitializeForPIE();
		}

		RenewLevelStreamingState();
	}
	else
	{
		StartPlayManuallyProc();
	}

	AZGameInstance->InitSocketOnMapLoad();

	FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(post_load_map_delegate_handle_);
}

void UAZMapMgr::MapChannelMove(int32 map_rec_key, int32 channel_index)
{
	if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(AZGameInstance->GetGameMode()))
	{
		game_mode->RemoveAllAppearObjects();
	}

	SendMapEnter();
	UE_LOG(LogClass, Log, TEXT("[CHANNEL MOVE] map(%d), channel(%d)"), map_rec_key, channel_index);
}

void UAZMapMgr::OnMapEnter_Ack()
{
	is_map_entered_ = true;

	if (IsValid(current_map_))
	{
		current_map_->OnStart();
	}

	if (same_map_enter_ready_ && AZGameInstance->GetPlayer())
	{
		same_map_enter_ready_ = false;
		// FIXME ���ս� Ȯ���ϱ�
		//AZGameInstance->GetPlayer()->ReleaseMapMove();
	}

	if (UAZWidget_InGame* ui = AZGameInstance->GetHUD()->GetUI<UAZWidget_InGame>(EUIName::AZWidget_InGame))
	{
		// FIXME ����Ʈ �߰��� UI�����ϱ�
		/*if (AZGameInstance->QuestMgr->GetStartQuestInformIndex() > 0 && GetCurrentMapType() != EMapType::RealmWar)
		{
			ui->SetLeftTabVisible(false);
			AZGameInstance->QuestMgr->TotalUpdateQuestUI();
		}
		else
		{
			AZGameInstance->QuestMgr->TotalUpdateQuestUI();
		}*/
	}

	// 
	/*if (IsAutoMovePortalReserved())
	{
		// FIXME �����̵� ����

		ResetAutoMovePortalInfo();
	}
	// FIXME �ڵ��̵��� ���߿�(Ǯ������ ���� ������
	else if (save_auto_move_data.MapIdx != -1)
	{
		SetAutoMove(saved_auto_move_data.map_index, saved_auto_move_data.auto_move_type, saved_auto_move_data.target_location);
	}
	*/
}

void UAZMapMgr::OnMapExit()
{
	AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(UGameplayStatics::GetGameMode(AZGameInstance->GetWorld()));
	if (game_mode)
	{
		game_mode->OnMapExit();
	}

	// FIXME Area ���� �ʱ�ȭ
	//MapArea.Empty();

	// FIXME ��Ż���� �ʱ�ȭ
	//PortalMoveTriggerList.Empty();

	is_map_entered_ = false;

	// FIXME Character���� ���� ������ �˸�
	// AZGameInstance->Characdter_Mgr->OnMapExit();
}

bool UAZMapMgr::OnRequestMapOut()
{
	if (current_map_ == nullptr)
	{
		return false;
	}

	return current_map_->OnRequestMapOut();
}

void UAZMapMgr::SendMapEnter()
{
	if (IsMapEnterRequestReserved())
	{
		// FIXME �� ���� �޽���
		//FCG_MAP_ENTER_REQUEST_CMD msg;
		//msg.map_index = reserved_map_enter_request_.map_index;
		ResetMapEnterRequest();
		// FIXME ������� ���� Ȯ���ϱ�
		//OnRecvMapEnterRequest(msg, true);
		return;
	}

	if (current_map_ == nullptr)
	{
		return;
	}

	// FIXME �պ��� Ȯ���ϱ�
	/*UAZSocketHolder* game_socket_holder = AZGameInstance->GetSocketHolder(ESocketHolderType::Gate);
	if (game_socket_holder != nullptr)
	{
		FCG_MAP_ENTER_SYN msg_syn;
		msg_syn.map_index = GetMapIndex();
		game_socket_holder->SendPacket(&msg_syn);
	}*/

	// FIXME �պ��� Ȯ���ϱ�
	/*
	if (AZGameInstance && AZGameInstance->GetPlayer())
	{
		AZGameInstance->GetPlayer()->OnMapEnter();
	}
	*/

	AZGameInstance->GetGameMode()->OnMapEnter();
}

void UAZMapMgr::StartPlayManuallyProc()
{
	if (AZGameInstance->GetGameMode()->CollectAsyncLoadPackage() <= 0)
	{
		// FIXME �ַ� �������� Ȯ���ϱ�
		/*if (GetCurrentMapType() == EMapType::AloneQUestDungeon)
		{
			PreLoadResources_Sync();
		}*/

		if (AZGameInstance->GetWorld()->WorldComposition)
		{
			AZGameInstance->GetWorld()->PopulateStreamingLevelsToConsider();
			FlushStreaming();
		}
		else
		{
			AZGameInstance->GetGameMode()->SetGamePaused(false);
			AZ_LOG("No WorldComposition in AAZGameMode_InGame");
		}

		AZGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::FirstTickAfterStartPlayManuallyProc));
	}
}

void UAZMapMgr::MapDestroy()
{
	if (dungeon_info_ == current_map_)
	{
		dungeon_info_ = nullptr;
	}

	if (current_map_)
	{
		current_map_->OnMapExit();
		current_map_->ConditionalBeginDestroy();
	}
	current_map_ = nullptr;
}

void UAZMapMgr::MapExit_Syn()
{
	// FIXME ���� �� Ȯ���ϱ�
	/*if (UAZSocketHolder* game_socket_holder = AZGameInstance->GetSocketHolder(ESocketHolderType::Game))
	{
		FCG_MAP_EXIT_SYN msg_syn;
		msg_syn.is_escape = false;
		game_socket_holder->SendPacket(&msg_syn);
	}*/
}

void UAZMapMgr::FirstTickAfterStartPlayManuallyProc()
{
	AZGameInstance->GetGameMode()->StartPlayManually();
	RenewLevelStreamingState(false, AZGameInstance->GetPlayer(), true);
	AZGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::SecondTickAfterStartPlayManuallyProc));
}

void UAZMapMgr::SecondTickAfterStartPlayManuallyProc()
{
	if (!AZGameInstance->GetWorld()->WorldComposition)
	{
		StopCapture();
		AZGameInstance->GetHUD()->OnFadeInOut(0.5f, 0.5f);
		FTimerHandle handle;
		AZGameInstance->GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateUObject(this, &UAZMapMgr::ThirdTickAfterStartPlayManuallyProc), 0.5f, false);
		SendMapEnter();
	}

	if (IsInDungeon())
	{
		OnBeginDungeon();
	}
}

void UAZMapMgr::ThirdTickAfterStartPlayManuallyProc()
{
	CloseLoadingScene();
	AZGameInstance->GetGameMode()->LoadSyncExtraLevels();

	if (reserved_force_kick_ != EForceKick::Max)
	{
		AZGameInstance->login_mgr->OnForceKicked(reserved_force_kick_);
		reserved_force_kick_ = EForceKick::Max;
	}
}

void UAZMapMgr::ProcessDisappearObject(int32 object_serial)
{
	if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(AZGameInstance->GetGameMode()))
	{
		auto* object = game_mode->FindAppearActors(object_serial);

		bool success = game_mode->RemoveAppearCharacter(object_serial);
		if (success == true)
		{
			UE_LOG(LogClass, Log, TEXT("[FCG_MAP_DISAPPEAR_OBJECT_CMD][M:%d] Disappear Character"), object_serial)
		}
		else
		{
			success = game_mode->RemoveAppearActor(object_serial);
			if (success == true)
			{
				UE_LOG(LogClass, Log, TEXT("disappear_object[m:%d] disappear actor"), object_serial);
			}
			else
			{
				UE_LOG(LogClass, Warning, TEXT("disappear_object[m:%d] not found actor"), object_serial);
			}
		}

		FDestroyedObject msg(object_serial);
		msg.object_ptr = object;
		AZGameInstance->OnGameMsg(&msg);

		// FIXME ���ս� �߰��ϱ�
		/*if (AAZPlayer_Playable* player = AZGameInstance->GetPlayer())
		{
			player->TargetFinder->OnTargetDisappeared(object_serial);
		}*/
	}
}

bool UAZMapMgr::IsInMapType(EMapType type) const
{
	if (IsInMap() == false)
	{
		return false;
	}
	EMapType map_type = UAZUtility::StringToEnum<EMapType>(GetMapData()->map_type);
	if (map_type != type)
	{
		return false;
	}
	return true;
}

int32 UAZMapMgr::GetMapIndex() const
{
	const auto map_data = GetMapData();
	return map_data ? map_data->id : 0;
}

UAZMapBase* UAZMapMgr::CreateMap(int32 map_index)
{
	if (auto map_data = GetTableByIndex(UMapData, map_index))
	{
		EMapType map_type = UAZUtility::StringToEnum<EMapType>(map_data->map_type);
		switch(map_type)
		{
		case EMapType::Dungeon:
			return NewObject<UAZDungeon>(this);
		default:
			return NewObject<UAZMapBase>(this);
		}
	}
	return nullptr;
}

void UAZMapMgr::OnRecvMapEnterRequest(const FCG_MAP_ENTER_REQUEST_CMD& recv_msg, bool reserved)
{
	// FIXME ���ս� Ǯ��
	/*if (reserved == false && IsOnLoading() && GetCurrentMapType() != EMapType::Prologue && AZGameInstance->map_channel_info.map_index != recv_msg.map_index)
	{
		ReserveMapEnterRequest(recv_msg);
		return;
	}

	if (AZGameInstance->map_channel_info.map_index == recv_msg.map_index)
	{
		// ���� ���� ä�� �̵�
		MapChannelMove(recv_msg.map_index, recv_msg.channel_index);

		// ��ġ �̵�
		if (AAZGameMode_InGame* game_mode = Cast<AAZGameMode_InGame>(AZGameInstance->GetGameMode()))
		{
			if (AAZPlayer_Playable* player = game_mode->GetPlayablePlayer())
			{

			}
		}
	}*/
}

void UAZMapMgr::ReserveMapEnterRequest(const FCG_MAP_ENTER_REQUEST_CMD& msg)
{
	reserved_map_enter_request_.map_index;
	reserved_map_enter_request_.channel_index;
	reserved_map_enter_request_.player_count;
	reserved_map_enter_request_.pos;
	reserved_map_enter_request_.look_at_angle;
	reserved_map_enter_request_.warp_type;
}

void UAZMapMgr::ResetMapEnterRequest()
{
	reserved_map_enter_request_.map_index = 0;
	reserved_map_enter_request_.channel_index = 0;
	reserved_map_enter_request_.player_count = 0;
	reserved_map_enter_request_.pos = FVector::ZeroVector;
	reserved_map_enter_request_.look_at_angle = 0.0f;
	reserved_map_enter_request_.warp_type = EWarpType::None;
}

bool UAZMapMgr::IsMapEnterRequestReserved() const
{
	return reserved_map_enter_request_.map_index > 0;
}

EMapType UAZMapMgr::GetCurrentMapType()
{
	const auto map_data = GetMapData();
	if (map_data == nullptr)
	{
		return EMapType::Max;
	}
	EMapType type = UAZUtility::StringToEnum<EMapType>(map_data->map_type);
	return map_data ? type : EMapType::Max;
}

FVector UAZMapMgr::GetEnterPos() const
{
	return enter_pos_;
}

float UAZMapMgr::GetEnterAngle() const
{
	return enter_angle_;
}

void UAZMapMgr::ConsumeEnterParams(FVector& location, FRotator& rotation)
{
	location = enter_pos_;
	rotation = FRotator(0.f, enter_angle_, 0.f);
	enter_pos_ = FVector::ZeroVector;

}

bool UAZMapMgr::IsInDungeon()
{
	return IsValid(dungeon_info_);
}

void UAZMapMgr::OnBeginDungeon()
{
	if (IsInDungeon())
	{
		/*auto goal_info = dungeon_info_->GetCurrentMainGoalInfo();
		int32 dungeon_info_index = goal_info ? goal_info->id : 0;
		if (auto ui = AZGameInstance->GetHUD()->GetUI<UAZWidget_InGame>(EUIName::AZWidget_InGame))
		{
			//ui->RenewDungeonGoalInfo(dungeon_info_index, true);
		}

		UMapDefineData* data = GetTable(UMapDefineData, 0);
		if (UMapDefineData != nullptr)
		{
			float check_time = UMapDefineData->start_time;
			GetWorld()->GetTimerManager().SetTimer(PartyDungeonAutoBattleStartTimer, this, &UAZMapMgr::DungeonBattleStart, check_time);
		}*/
	}
}

void UAZMapMgr::UpdateStreaming()
{
	if (!AZGameInstance || !AZGameInstance->GetWorld() || !AZGameInstance->GetWorld()->WorldComposition || !AZGameInstance->GetPlayer())
	{
		return;
	}

	// FIXME Ȯ�� �ʿ�
	/*if (AZGameInstance->GetWorld()->bDisableUpdateStreamingState == false)
	{
		AZGameInstance->GetWorld()->bDisableUpdateStreamingState = true;
	}*/

	if (pause_streaming_ == true)
	{
		return;
	}

	if (current_streaming_state_ == EStreamingState::None)
	{
		RenewLevelStreamingState(false, AZGameInstance->GetPlayer());
		return;
	}

	switch (current_streaming_state_)
	{
	case EStreamingState::None:
	case EStreamingState::Paused:
		break;
	case EStreamingState::Normal:
	case EStreamingState::Preload:
		UpdateWorldCompositionStreamingState(true);
		break;
	case EStreamingState::Waiting:
	case EStreamingState::Postload:
	case EStreamingState::PrepareCinematic:
		UpdateStreamingCheck();
		break;

	case EStreamingState::OnCinematic:
		UpdateStreamingOnCinematic();
		break;
	}

	if (last_streaming_state_ != current_streaming_state_ && GetName().Contains("Default") == false)
	{
		switch (current_streaming_state_)
		{
		case EStreamingState::None: AZ_LOG("current streaming state : None"); break;
		case EStreamingState::Normal: AZ_LOG("current streaming state : Normal"); break;
		case EStreamingState::Preload: AZ_LOG("current streaming state : Preload"); break;
		case EStreamingState::Waiting: AZ_LOG("current streaming state : Waiting"); break;
		case EStreamingState::Postload: AZ_LOG("current streaming state : Postload"); break;
		case EStreamingState::Paused: AZ_LOG("current streaming state : Paused"); break;
		case EStreamingState::PrepareCinematic: AZ_LOG("current streaming state : "); break;
		case EStreamingState::OnCinematic: AZ_LOG("current streaming state : OnCinematic"); break;
		}
	}

	last_streaming_state_ = current_streaming_state_;
}

void UAZMapMgr::UpdateWorldCompositionStreamingState(bool include_player)
{
	TArray<FVector> locations;
	if (include_player == true)
	{
		FVector location;
		FRotator rot;
		if (character_only_streaming_ == false)
		{
			AZGameInstance->GetPlayerController()->GetPlayerViewPoint(location, rot);
			locations.Add(location);
		}
		locations.Add(AZGameInstance->GetPlayer()->GetActorLocation());
	}
	bool include_additional_location = false;
	switch (current_streaming_state_)
	{
	case EStreamingState::Preload:
	case EStreamingState::Waiting:
	case EStreamingState::Postload:
	case EStreamingState::PrepareCinematic:
		include_additional_location = true;
		break;
	}
	if (include_additional_location)
	{
		locations.Add(level_streaming_position_);
	}

	if (locations.Num() > 0)
	{
		if (AZGameInstance->GetPlayerController() && AZGameInstance->GetPlayerController()->bCinematicMode)
		{
			AZGameInstance->GetWorld()->WorldComposition->UpdateStreamingStateCinematic(locations.GetData(), locations.Num());
		}
		else
		{
			AZGameInstance->GetWorld()->WorldComposition->UpdateStreamingState(locations.GetData(), locations.Num());
		}
	}
}

void UAZMapMgr::UpdateStreamingOnCinematic()
{
	if (AZGameInstance->GetPlayerController() && AZGameInstance->GetPlayerController()->bCinematicMode)
	{
		AZGameInstance->GetWorld()->WorldComposition->UpdateStreamingStateCinematic(cinematic_camera_trajectories_.GetData(), cinematic_camera_trajectories_.Num());
	}
	else
	{
		AZGameInstance->GetWorld()->WorldComposition->UpdateStreamingState(cinematic_camera_trajectories_.GetData(), cinematic_camera_trajectories_.Num());
	}
}

void UAZMapMgr::UpdateStreamingCheck()
{
	if (AZGameInstance == nullptr)
	{
		return;
	}

	if (current_streaming_state_ != Waiting && current_streaming_state_ != Postload && current_streaming_state_ != PrepareCinematic)
	{
		return;
	}

	if (auto* world = AZGameInstance->GetWorld())
	{
		if (world->WorldComposition == nullptr)
		{
			return;
		}

		bool end_waiting = true;

		auto streaming_levels = world->GetStreamingLevels();

		int count = streaming_levels.Num();

		for (int i = 0; i < count; ++i)
		{
			auto streaming_level = streaming_levels[i];

			if (streaming_level->GetWorldAssetPackageName().Contains(TEXT("_Design_")))
			{
				continue;
			}

			if (streaming_level->ShouldBeLoaded() == false || streaming_level->ShouldBeVisible() == false)
			{
				continue;
			}

			if (current_streaming_state_ == PrepareCinematic)
			{
				// FIXME Ȯ���ʿ�
			}
			else
			{
				if (streaming_level->GetStreamingVolumeBounds().IsValid == false)
				{
					if (streaming_level->bDisableDistanceStreaming == false)
					{
						auto dynamic_streaming_levels = AZGameInstance->GetGameMode()->GetAsyncLoadableStreamingTile();
						if (dynamic_streaming_levels.Contains(streaming_level) == false)
						{
							continue;
						}
					}
				}
				else
				{
					if (IsInBoundAtLeastOneLocation(streaming_level->GetStreamingVolumeBounds()) == false)
					{
						continue;
					}
				}
			}

			if (streaming_level->IsStreamingStatePending())
			{
				// �ϳ��� ��� �ɷ������� ��ٸ�
				end_waiting = false;
				break;
			}
		}

		if (FShaderPipelineCache::NumPrecompilesRemaining() > 0)
		{
			end_waiting = false;
		}

		if (current_streaming_state_ == Waiting)
		{
			if (end_waiting)
			{
				//FIXME ���ս� Ȯ��
				if (/*HasFloor(AZGameInstance->GetPlayer())*/true)
				{
					if (AZGameInstance->GetGameMode()->HasActorBegunPlay())
					{
						OnFinishStreamingLevelShown();
					}
					else
					{
						current_streaming_state_ = Paused;
						AZGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::OnFinishStreamingLevelShown));
					}
				}
				else
				{
					UpdateWorldCompositionStreamingState(false);
					waiting_max_frame_count_++;
				}
			}
			else
			{
				UpdateWorldCompositionStreamingState(false);
				waiting_max_frame_count_++;
			}

			if (waiting_max_frame_count_ >= 300)
			{
				if (AZGameInstance->GetGameMode()->HasActorBegunPlay())
				{
					OnFinishStreamingLevelShown();
				}
				else
				{
					current_streaming_state_ = Paused;
					AZGameInstance->GetWorld()->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateUObject(this, &UAZMapMgr::OnFinishStreamingLevelShown));
				}
			}
		}
		else if (current_streaming_state_ == Postload)
		{
			check_frame_after_streaming_++;
			if (check_frame_after_streaming_ < 120)
			{
				if (end_waiting == false)
				{
					RenewLevelStreamingState(false, AZGameInstance->GetPlayer());
				}
				else
				{
					UpdateWorldCompositionStreamingState(true);
				}
			}
			else
			{
				current_streaming_state_ = Normal;
				UpdateWorldCompositionStreamingState(true);
				// �Ϻ��� �Ϸ�
			}
		}
		else if (current_streaming_state_ == PrepareCinematic)
		{
			if (end_waiting || waiting_max_frame_count_ >= 300)
			{
				current_streaming_state_ = OnCinematic;
				AZGameInstance->GetGameMode()->SetGamePaused(false);
			}
			else
			{
				UpdateStreamingOnCinematic();
				waiting_max_frame_count_++;
			}
		}
	}
}

void UAZMapMgr::UpdateLoadingLightHiddenCheck()
{
	if (!AZGameInstance || !AZGameInstance->GetWorld() || !AZGameInstance->GetWorld()->WorldComposition || !AZGameInstance->GetPlayer())
	{
		return;
	}

	if (is_loading_ == false)
	{
		return;
	}

	if (AZGameInstance->GetWorld())
	{
		TArray<AActor*> actors;

		UGameplayStatics::GetAllActorsOfClass(AZGameInstance->GetWorld(), AActor::StaticClass(), actors);
		for (int i = 0; i < actors.Num(); ++i)
		{
			if (actors[i]->ActorHasTag(TAG_LoadingCheck))
			{
				continue;
			}

			actors[i]->Tags.Emplace(TAG_LoadingCheck);

			if (Cast<AAZActor_LoadingScene>(actors[i]))
			{
				continue;
			}

			if (auto volume = Cast<APostProcessVolume>(actors[i]))
			{
				if (volume->bEnabled)
				{
					volume->bEnabled = false;
					volume->SetActorHiddenInGame(true);

					loading_hidden_volume_actors_.Emplace(volume);
				}
			}

			TArray<UActorComponent*> components;

			actors[i]->GetComponents(ULightComponent::StaticClass(), components, true);
			// �� ������Ʈ �ε� �Ѿ��
			for (int j = 0; j < components.Num(); ++j)
			{
				if (auto light = Cast<ULightComponent>(components[j]))
				{
					light->SetVisibility(false);
					light->SetHiddenInGame(true);

					loading_hidden_light_components_.Emplace(light);
				}
			}

			actors[i]->GetComponents(USkyLightComponent::StaticClass(), components, true);
			for (int j = 0; j < components.Num(); ++j)
			{
				if (auto light = Cast<USkyLightComponent>(components[j]))
				{
					light->SetVisibility(false);
					light->SetHiddenInGame(true);

					loading_hidden_light_components_.Emplace(light);
				}
			}
		}
	}
}

void UAZMapMgr::RenewLevelStreamingState(bool force_flush, /*FIXME ���ս� Ȯ��*/ACharacter* player, bool close_loading_scene_on_finish)
{
	UWorld* world = AZGameInstance->GetWorld();

	if (world == nullptr)
	{
		return;
	}

	if (world->WorldComposition == nullptr)
	{
		return;
	}

	current_streaming_state_ = Waiting;

	waiting_max_frame_count_ = 0;

	close_loading_scene_on_finish_ = close_loading_scene_on_finish;

	if (AZGameInstance->GetGameMode()->HasActorBegunPlay())
	{
		AZGameInstance->GetGameMode()->SetGamePaused(true);
	}

	level_streaming_position_ = player ? player->GetActorLocation() : enter_pos_;

	UpdateWorldCompositionStreamingState(false);

	if (force_flush || flush_streaming_)
	{
		FlushStreaming();
		OnFinishStreamingLevelShown();
	}
}

void UAZMapMgr::OnFinishStreamingLevelShown()
{
	UWorld* world = AZGameInstance->GetWorld();

	if (!world || !world->WorldComposition)
	{
		return;
	}

	// FIXME ���ս� Ȯ��
	if (renew_streaming_recursive_count_ < 10 /*&& HasFloor(AZGameInstance->GetPlayer()) == false*/)
	{
		++renew_streaming_recursive_count_;
		RenewLevelStreamingState(false, AZGameInstance->GetPlayer(), close_loading_scene_on_finish_);
		return;
	}

	if (AZGameInstance->GetGameMode()->HasActorBegunPlay())
	{
		AZGameInstance->GetGameMode()->SetGamePaused(false);
	}
	else
	{
		StartPlayManuallyProc();
	}

	TArray<AActor*> characters;
	// FIXME ���ս� ĳ���ͷ� �����ϱ�
	UGameplayStatics::GetAllActorsOfClass(world, ACharacter::StaticClass(), characters);

	for (int32 i = 0; i < characters.Num(); ++i)
	{
		// FIXME ���ս� ĳ���ͷ� �����ϱ�
		/*if (auto character = Cast<AAZCharacter>(characters[i]))
		{
			if (AAZPlayer_Playable* player = Cast<AAZPlayer_Playable>(character))
			{
				character->AdjustStandPosition();
			}
		}*/
	}

	if (close_loading_scene_on_finish_ && IsOnLoading())
	{
		StopCapture();
		AZGameInstance->GetHUD()->OnFadeInOut(0.5f, 0.5f);
		FTimerHandle handle;
		GetWorld()->GetTimerManager().SetTimer(handle, FTimerDelegate::CreateUObject(this, &UAZMapMgr::ThirdTickAfterStartPlayManuallyProc), 0.5f, false);
	}

	close_loading_scene_on_finish_ = false;

	// FIXME ���ս� Ȯ���ʿ�
	/*if (AZGameInstance->GetPlayer())
	{
		AZGameInstance->GetPlayer()->GetAZCharacterMovement_Sync()->SetIgnoreSync(false);
	}*/

	
	// FIXME ���ս� ĳ���� ���� ����ȭ Ȯ��üũ�ϱ�.
	/*if (ready_server_sync_streaming_)
	{
		if (use_teleport_after_sync_streaming)
		{
			character_state state = AZGameInstance->GetPlayer()->GetCharacterState() == ECharacterState::WarpFly ? ECharacterState::WarpFlyEnd : ECharacterState::WarpEnd;
			AZGameInstance->GetPlayer()->SetCharacterState(state);
		}

		// FIXME ���� �޽���
	}*/

	ready_server_sync_streaming_ = false;
	use_teleport_after_sync_streaming_ = false;

	current_streaming_state_ = Postload;
	check_frame_after_streaming_ = 0;
	renew_streaming_recursive_count_ = 0;
}

void UAZMapMgr::OnRecvWarpRequest(bool is_teleport)
{
	if (AZGameInstance == nullptr || AZGameInstance->GetWorld() == nullptr)
	{
		return;
	}

	if (AZGameInstance->GetWorld()->WorldComposition)
	{
		ready_server_sync_streaming_ = true;
		if (close_loading_scene_on_finish_ == false)
		{
			close_loading_scene_on_finish_ = is_teleport;
		}
	}
	else
	{
		// FIXME ���ս� üũ �ʿ�
		/*if (is_teleport && AZGameInstance->GetPlayer())
		{
			ECharacterState state = AZGameInstance->GetPlayer()->GetCharacterState() == ECharacterState::WarpFly ? ECharacterState::WarpFlyEnd : ECharacterState::WarpEnd;
			AZGameInstance->GetPlayer()->SetCharacterState(state);
		}*/

		//FixMe ���� End �޽���
		/*if (ULHSocketHolder* socketHolder = LHGameInstance->GetSocketHolder(ESocketHolderType::Gate))
		{
			FCG_MAP_WARP_ENTER_SYN msg;
			socketHolder->SendPacket(&msg);
		}*/
	}
}

void UAZMapMgr::SetPrepareCinmaticStreaming(const TArray<FVector>& cinematic_locations)
{
	if (AZGameInstance->GetWorld() == nullptr || AZGameInstance->GetWorld()->WorldComposition == nullptr)
	{
		return;
	}

	cinematic_camera_trajectories_.Empty();

	{
		FVector loc;
		FRotator rot;
		AZGameInstance->GetPlayerController()->GetPlayerViewPoint(loc, rot);
		cinematic_camera_trajectories_.Add(loc);
		cinematic_camera_trajectories_.Add(AZGameInstance->GetPlayer()->GetActorLocation());
	}
	cinematic_camera_trajectories_.Append(cinematic_locations.GetData(), cinematic_locations.Num());

	waiting_max_frame_count_ = 0;
	AZGameInstance->GetGameMode()->SetGamePaused(true);
	current_streaming_state_ = PrepareCinematic;

	UpdateStreamingOnCinematic();
}

void UAZMapMgr::FinishCinematicStreaming()
{
	if (current_streaming_state_ != OnCinematic && current_streaming_state_ != PrepareCinematic)
	{
		return;
	}

	RenewLevelStreamingState(true, AZGameInstance->GetPlayer());
}

void UAZMapMgr::OpenLoadingScene(int32 map_index)
{
	if (loading_map_index_ == map_index)
	{
		return;
	}

	loading_map_index_ = map_index;

	if (loading_widget_)
	{
		//FixMe ���ս� �����ϱ�
		auto info = GetTable(UModelData);
		const UModelData* model_data = *info.GetData();
		SyncLoadingActorInstance(model_data, map_index);
		loading_widget_->SetLoadingModel(model_data, map_index);
		return;
	}

	// FIXME ��� Ȯ���ϱ�
	FString path = TEXT("/Game/FIXME..");
	UClass* load_class = AZResourceHelper::LoadClassFast<UAZWidget>(path);
	if (load_class)
	{
		loading_widget_ = Cast<UAZWidget_Loading>(UUserWidget::CreateWidgetInstance(*AZGameInstance, load_class, TEXT("LoadingScene")));
		if (loading_widget_)
		{
			loading_widget_->Init();
			loading_widget_->AddToViewport((int32)EUILayer::Max);
			loading_widget_->OnOpen();

			//FixMe ���ս� �����ϱ�
			auto info = GetTable(UModelData);
			const UModelData* model_data = *info.GetData();
			SyncLoadingActorInstance(model_data, map_index);
			loading_widget_->SetLoadingModel(model_data, map_index);

			TArray<AActor*> actors;
			UGameplayStatics::GetAllActorsOfClass(AZGameInstance->GetWorld(), AActor::StaticClass(), actors);
			for (int i = 0; i < actors.Num(); ++i)
			{
				actors[i]->Tags.Emplace(TAG_LoadingCheck);
			}

			is_loading_ = true;
		}
		load_class = nullptr;
	}
}

void UAZMapMgr::CloseLoadingScene()
{
	if (loading_widget_ != nullptr)
	{
		return;
	}

	for (int i = 0; i < loading_hidden_volume_actors_.Num(); ++i)
	{
		if (loading_hidden_volume_actors_[i])
		{
			loading_hidden_volume_actors_[i]->bEnabled = true;
			loading_hidden_volume_actors_[i]->SetActorHiddenInGame(false);
		}
	}
	loading_hidden_volume_actors_.Empty();

	for (int i = 0; i < loading_hidden_volume_actors_.Num(); ++i)
	{
		if (IsValid(loading_hidden_light_components_[i]))
		{
			loading_hidden_light_components_[i]->SetVisibility(true);
			loading_hidden_light_components_[i]->SetHiddenInGame(false);
		}
	}
	loading_hidden_light_components_.Empty();

	is_loading_ = false;
	loading_map_index_ = 0;

	ClearLoadingActor();
	loading_widget_->RemoveFromParent();
	loading_widget_->MarkAsGarbage();
	loading_widget_->ConditionalBeginDestroy();
	loading_widget_ = NULL;

	// FIXME ���� �۾��ϱ�
	// �ε������� üũ�ѹ�
	/*if (IsValid(AZGameInstance->SoundAlarmMgr))
	{
		AZGameInstance->SoundAlarmMgr->CheckAllAlarm(true);
	}*/
}

void UAZMapMgr::StopCapture()
{
	if (IsValid(loading_scene_actor_instance_))
	{
		loading_scene_actor_instance_->StopCapture();
	}
}

void UAZMapMgr::ClearLoadingActor()
{
	
	if (IsValid(loading_scene_actor_instance_))
	{
		loading_scene_actor_instance_->MarkAsGarbage();
		loading_scene_actor_instance_->ConditionalBeginDestroy();
		loading_scene_actor_instance_ = nullptr;
	}
}

bool UAZMapMgr::IsOnLoading() const
{
	return IsValid(loading_widget_) && loading_widget_->IsInViewport();
}

void UAZMapMgr::FlushStreaming()
{
	if (IsValid(AZGameInstance->GetWorld()) == false)
	{
		return;
	}

	if (AZGameInstance->GetWorld()->WorldComposition == nullptr)
	{
		return;
	}

	AZGameInstance->GetWorld()->UpdateLevelStreaming();
	FlushAsyncLoading();
	AZGameInstance->GetWorld()->UpdateLevelStreaming();

	auto streaming_levels = AZGameInstance->GetWorld()->GetStreamingLevels();

	while (true)
	{
		bool end_waiting = true;
		for (int i = 0, cnt = streaming_levels.Num(); i < cnt; ++i)
		{
			auto streaming_level = streaming_levels[i];

			if (streaming_level->GetWorldAssetPackageName().Contains(TEXT("_Design_")))
			{
				continue;
			}

			if (streaming_level->ShouldBeLoaded() == false || streaming_level->ShouldBeVisible() == false)
			{
				continue;
			}

			streaming_level->GetStreamingVolumeBounds();
			if (streaming_level->GetStreamingVolumeBounds().IsValid == false)
			{
				if (streaming_level->bDisableDistanceStreaming == false)
				{
					auto dynamic_streaming_levels = AZGameInstance->GetGameMode()->GetAsyncLoadableStreamingLevel();
					if (dynamic_streaming_levels.Contains(streaming_level) == false)
					{
						continue;
					}
				}
			}
			else
			{
				if (IsInBoundAtLeastOneLocation(streaming_level->GetStreamingVolumeBounds()) == false)
				{
					continue;
				}
			}

			if (streaming_level->IsStreamingStatePending())
			{
				end_waiting = false;
				break;
			}
		}

		if (end_waiting == true)
		{
			break;
		}

		FlushAsyncLoading();
		AZGameInstance->GetWorld()->UpdateLevelStreaming();
	}
}

bool UAZMapMgr::IsInBoundAtLeastOneLocation(const FBox& bound) const
{
	TArray<FVector> locations = GetStreamingLocations();
	for (int i = 0; i < locations.Num(); ++i)
	{
		if (bound.IsInsideOrOn(locations[i]))
		{
			return true;
		}
	}
	return false;
}

TArray<FVector> UAZMapMgr::GetStreamingLocations() const
{
	bool include_player = true;
	switch (current_streaming_state_)
	{
	case EStreamingState::Waiting:
		include_player = false;
		break;
	}

	bool include_additional_location = false;
	switch (current_streaming_state_)
	{
	case EStreamingState::Preload:
	case EStreamingState::Waiting:
	case EStreamingState::Postload:
	case EStreamingState::PrepareCinematic:
		include_additional_location = true;
		break;
	}

	TArray<FVector> locations;
	if (include_player == true)
	{
		FVector loc;
		FRotator rot;
		AZGameInstance->GetPlayerController()->GetPlayerViewPoint(loc, rot);
		locations.Add(loc);
		// FIXME ���ս� ���� �ʿ�.
		//locations.Add(AZGameInstance->GetPlayer()->GetActorLocation());
	}
	if (include_additional_location)
	{
		// FIXME ���ս� level streaming position ��ġ Ȯ���ϱ�
		locations.Add(level_streaming_position_);
	}
	return locations;
}

void UAZMapMgr::AddSubLevelFromPackageName(const FName& package_name, UPackage* loaded_package, EAsyncLoadingResult::Type result)
{
	if (GetWorld()->WorldComposition == nullptr)
	{
		TArray<ULevelStreaming*> async_loadable_streaming_list = AZGameInstance->GetGameMode()->GetAsyncLoadableStreamingLevel();
		for (int32 i = 0; i < async_loadable_streaming_list.Num(); ++i)
		{
			if (async_loadable_streaming_list[i]->GetWorldAssetPackageFName() == package_name)
			{
				async_loadable_streaming_list[i]->SetShouldBeLoaded(true);
				async_loadable_streaming_list[i]->SetShouldBeVisible(true);

				AZ_LOG("[UAZMapMgr::AddSubLevelFromPackageName] %s", *package_name.ToString());
				break;
			}
		}
	}
	else
	{
		TArray<ULevelStreaming*> async_loadable_streaming_list = AZGameInstance->GetGameMode()->GetAsyncLoadableStreamingLevel();
		for (int32 i = 0; i < async_loadable_streaming_list.Num(); ++i)
		{
			if (async_loadable_streaming_list[i]->GetWorldAssetPackageFName() == package_name)
			{
				bool success;
				auto level = ULevelStreamingDynamic::LoadLevelInstanceBySoftObjectPtr(GetWorld(), async_loadable_streaming_list[i]->GetWorldAsset(), FVector::ZeroVector, FRotator::ZeroRotator, success);
				level->SetShouldBeLoaded(true);

				AZ_LOG("[UAZMapMgr::AddSubLevelFromPackageName] %s (%s)", *package_name.ToString(), (success ? TEXT("Success") : TEXT("Failed")));
				break;
			}
		}
	}
}

bool UAZMapMgr::HasFloor(AAZCharacter* character)
{
	// FIXME ���ս� �ٴ�üũ �ֱ�
	/*float traceZ = character->GetLHCharacterMovement()->ClinetZ_TraceStartZ;
	FVector location = character->GetActorServerLocation();
	FVector start = location + FVector::UpVector * traceZ;
	FVector end = location - FVector::UpVector * traceZ;
	TArray<AActor*> actorsToIgnore;
	FHitResult outHit;

	bool result = false;

	result = UKismetSystemLibrary::LineTraceSingle(character, start, end, UEngineTypes::ConvertToTraceType(LH_Trace_Floor), false, actorsToIgnore, EDrawDebugTrace::None, outHit, true);
	if (result)
	{
		return true;
	}

	result = UKismetSystemLibrary::LineTraceSingle(character, start, end, UEngineTypes::ConvertToTraceType(LH_TRACE_SWIM), false, actorsToIgnore, EDrawDebugTrace::None, outHit, true);
	if (result)
	{
		return true;
	}*/

	return false;
}

void UAZMapMgr::SyncLoadingActorInstance(const UModelData* model_data, int32 map_index)
{
	if (model_data == nullptr)
	{
		return;
	}

	TArray<AActor*> temp_array;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAZActor_LoadingScene::StaticClass(), temp_array);
	if (temp_array.Num() <= 0)
	{
		FVector spawn_location;
		auto loading_data = GetGameSingleton()->table_mgr->GetData<ULoadingData>([map_index](const ULoadingData* check) {
			if (check->map_index == map_index)
			{
				return true;
			}
			return false;
			});

		auto data = *loading_data.GetData();
		if (loading_data.Num() > 0)
		{
			spawn_location = FVector(data->location_x, data->location_y, data->location_z);

		}
		else
		{
			spawn_location = FVector::ZeroVector;
		}

		FString model_bp = (model_data->model_bp.Num() > 0 ? model_data->model_bp[FMath::Rand() % model_data->model_bp.Num()] : TEXT(""));
		// FIXME ���ս� ��� Ȯ���ϱ�
		FString loading_scene_path(TEXT("Environment/LoadingScene/"));
		FString load_path = FString(TEXT("/Game/Blueprint/")) + loading_scene_path + model_bp + "." + model_bp + "_C";
		FSoftClassPath path(load_path);
		UClass* load_class = path.TryLoadClass<AAZActor_LoadingScene>();
		loading_scene_actor_instance_ = GetWorld()->SpawnActor<AAZActor_LoadingScene>(load_class, spawn_location, FRotator::ZeroRotator);

		if (IsValid(loading_scene_actor_instance_) == false)
		{
			AZ_LOG("[UAZMapMgr::SyncLoadingActorInstance] Can't find ModelBP(%s)", *model_bp);
			return;
		}

		if (GetWorld()->HasBegunPlay() == false)
		{
			loading_scene_actor_instance_->DispatchBeginPlay();
		}

		EModelType model_type = UAZUtility::StringToEnum<EModelType>(model_data->model_type);
		if (model_type == EModelType::Avatar)
		{
			// FIXME 
			//if (UAvatarInfoRecord* avatarRecord = GetTable(UAvatarTable)->m_AvatarInfoSheet->GetRecord(modelInfo->ModelIndex))
			//{
			//	EClassType modelClassType = (EClassType)(avatarRecord->ClassType);
			//	LoadingSceneActorInstance->SetModelMesh(modelClassType, modelInfo->ModelIndex);
			//}
		}
		else
		{
			loading_scene_actor_instance_->SetCaptureCameraLocation();
		}
		loading_scene_actor_instance_->SetMergeCompVisible(model_type == EModelType::Avatar);
	}
	else if (temp_array.Num() > 0)
	{
		loading_scene_actor_instance_ = Cast<AAZActor_LoadingScene>(temp_array[0]);
	}
}
