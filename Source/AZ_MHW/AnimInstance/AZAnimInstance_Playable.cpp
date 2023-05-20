// Copyright Team AZ. All Rights Reserved.


#include "AZAnimInstance_Playable.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"
#include "AZ_MHW/PlayerState/AZPlayerState_Client.h"
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
		
		if(AAZPlayerState_Client* player_state =  Cast<AAZPlayerState_Client>(player->GetPlayerState()))
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

void UAZAnimInstance_Playable::AnimNotify_OnGetItem()
{
	//이펙트 ?
}
