// Copyright Team AZ. All Rights Reserved.


#include "AZAnimInstance_Playable.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "AZ_MHW/PlayerState/AZPlayerState.h"
#include "Character/Player/AZPlayer_Playable.h"

UAZAnimInstance_Playable::UAZAnimInstance_Playable()
{
	
}

void UAZAnimInstance_Playable::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
}

void UAZAnimInstance_Playable::NativeUpdateAnimation(float delta_seconds)
{
	if(AAZPlayer* player = Cast<AAZPlayer>(TryGetPawnOwner()))
	{
		player_cache_ = player;
		
		if(AAZPlayerState* player_state =  Cast<AAZPlayerState>(player->GetPlayerState()))
		{
			player_state_cache_ = player_state;

			//AnimInstance_Player에서 초기화.
			//current_forward_direction_ = player->GetRootComponent()->GetComponentRotation();

			//Playable 초기화
			next_forward_direction_ = player_state_cache_->action_state_.input_direction.ToOrientationRotator();
			//Remotable은 서버에서 초기화
			//next_forward_direction_ = GetRemotableDirection(id);
		}
	}

	//상태변화 체크
	//입력변화 체크
	//액션조건 체크
	//액션 선택
	//액션 실행화

	//회전보간, 전환확인
	Super::NativeUpdateAnimation(delta_seconds);
}

int32 UAZAnimInstance_Playable::GetInputActionBitMask() const
{
	int32 result = 0;//None
	
	if(player_state_cache_->action_state_.bit_move_forward) result |=(1<<0);//W
	if(player_state_cache_->action_state_.bit_move_left) result |=(1<<1);//A
	if(player_state_cache_->action_state_.bit_move_back) result |=(1<<2);//S
	if(player_state_cache_->action_state_.bit_move_right) result |=(1<<3);//D
	
	if(player_state_cache_->action_state_.bit_normal_action) result |=(1<<4);//MLB
	if(player_state_cache_->action_state_.bit_special_action) result |=(1<<5);//MRB
	if(player_state_cache_->action_state_.bit_evade_action) result |=(1<<6);//Space
	if(player_state_cache_->action_state_.bit_dash_action) result |=(1<<7);//LeftShift
	if(player_state_cache_->action_state_.bit_unique_action) result |=(1<<8);//LeftCtrl
	
	if(player_state_cache_->action_state_.bit_use_item) result |=(1<<9);//E
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<10);//V
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<11);//F
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<12);//R
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<13);//C
	//if(player_state_cache_->action_state_.bit_use_item_) result |=(1<<14);//M
	
	return result;
}

bool UAZAnimInstance_Playable::GetResultBitMask(const int32 input, const int32 bitmask) const
{
	return ((input & bitmask) > 0);
}

void UAZAnimInstance_Playable::AnimNotify_OnUseItem()
{
	if(player_cache_)
	{
		if(auto player = Cast<AAZPlayer_Playable>(player_cache_))
		{
			player->AnimNotify_OnUseItem();
		}
	}
}
