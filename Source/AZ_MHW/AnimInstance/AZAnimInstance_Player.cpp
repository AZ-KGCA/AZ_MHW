// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/AnimInstance/AZAnimInstance_Player.h"
#include "AZ_MHW/GameSingleton/AZGameSingleton.h"
#include "AZ_MHW/Manager/AZPlayerMgr.h"
#include "Character/AZCharacter.h"
#include "PlayerState/AZPlayerState.h"

UAZAnimInstance_Player::UAZAnimInstance_Player()
{
	//기본설정은 몽타주를 재생하는 것.
	is_montage_ = true;
	is_rotation_ = false;
	current_anim_play_rate_ = 2.f;
	handle_animation_transition_trigger_.BindLambda(
		[this](UAZAnimInstance* T) -> bool
		{
			return false;
		});
}
void UAZAnimInstance_Player::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	//플레이어 에셋매니저에서 Base 몽타주를 가져와서 할당
	if(const auto& anim_montage_asset = UAZGameSingleton::instance()->player_mgr_->GetMontage(TEXT("Base")))
	{
		current_anim_montage_ = anim_montage_asset;
		Montage_Play(current_anim_montage_, current_anim_play_rate_);
	}
}

void UAZAnimInstance_Player::NativeUpdateAnimation(float delta_seconds)
{
	Super::NativeUpdateAnimation(delta_seconds);
	//if(TryGetPawnOwner()->GetPlayerState()) playable? remotable?
	if(owner_ != nullptr)
	{

		//Playable과 Remotable 공통사항
		current_forward_direction_ = owner_->GetRootComponent()->GetComponentRotation();

		//playable
		//next_forward_direction = player controller

		//remotable
		//next_forward_direction = server 

		if(is_rotation_)//애니메이션이 회전보간을 사용할때(이동, 정지회전)
		{
			// if(auto player_state = owner_->GetPlayerState())
			// {
			// 	if(auto az_player_state = Cast<AAZPlayerState>(player_state))
			// 	{
			// 		FRotator owner_forward_direction = az_player_state->action_state_.input_direction.ToOrientationRotator();
			// 	}
			// }

			const FQuat next_quaternion(next_forward_direction_);
			const FQuat current_quaternion(current_forward_direction_);
			owner_->GetRootComponent()->SetWorldRotation( FQuat::Slerp(next_quaternion, current_quaternion,0.9).Rotator());

			//회전 보간 종료
			if(FMath::Abs(next_forward_direction_.Yaw) -  FMath::Abs(current_forward_direction_.Yaw) < 0.1f)
			{
				is_rotation_ = false;
			}
		}
	}

	if(should_transition_)//즉시 전환 or 지연 전환()
	{
		OnAnimationTransitionTrigger();
		should_transition_ = false;
	}
	else//조건 전환
	{
		//매업데이트 종료조건 델리게이트를 체크한다.
		//기본은 false만 반환하여 특수 종료조건을 처리하지 않는다
		if(handle_animation_transition_trigger_.IsBound())
		{
			if(handle_animation_transition_trigger_.Execute(this))
			{
				OnAnimationTransitionTrigger();
			}
		}
	}
}

void UAZAnimInstance_Player::OnAnimationTransitionTrigger()
{
	if(is_montage_)//몽타주 모드
	{
		if(next_montage_name_ != NAME_None)//몽타주 변경
		{
			if(current_montage_name_ != next_montage_name_)//동일 몽타주일시 무시
			{
				if(const auto& next_montage_asset = UAZGameSingleton::instance()->player_mgr_->GetMontage(next_montage_name_))
				{
					Montage_Stop(0.5f, current_anim_montage_);
					Montage_Play(next_montage_asset, current_anim_play_rate_);

				
					current_anim_montage_ = next_montage_asset;

					current_montage_name_ = next_montage_name_;
				
				
					current_section_name_ = TEXT("Default");//일단 기본 섹션으로 지정
				}
			}
			next_montage_name_ = NAME_None;
		}
		
		if(next_section_name_ != NAME_None)//섹션 변경도 있다면
		{
			if(current_section_name_ != next_section_name_)//동일 섹션이 아니라면 실행
			{
				if(current_anim_montage_->IsValidSectionName(next_section_name_))//유효한 섹션 이름 체크
				{
					current_section_name_ = next_section_name_;

					//점프 투 섹션 엔드를 해야 노티파이들과 원활하게 작동되는데(끝지점에서 다른 체크가 필요한 애니메이션의 경우),
					//섹션의 끝 시점의 애니메이션 포즈가 출력되서... 튀는 모습이 연출되어서 제거
					//Montage_JumpToSectionsEnd(current_section_name_, current_anim_montage_);
					
					Montage_JumpToSection(current_section_name_, current_anim_montage_);
				}
			}
			next_section_name_ = NAME_None;
		}
		else//섹션변경이 없다면
		{
			current_section_name_ = TEXT("Default");//이름을 디폴트로
			Montage_JumpToSection(current_section_name_, current_anim_montage_);
		}
	}
	else//시퀀스 모드
	{
		if(next_sequence_name_ != NAME_None)//시퀀스 체크
		{
			if(const auto& next_sequence_asset = UAZGameSingleton::instance()->player_mgr_->GetSequence(next_sequence_name_))
			{
				current_anim_sequence_ = next_sequence_asset;
				current_sequence_name_ = next_sequence_name_;
				next_sequence_name_ = NAME_None;
			}
		}
	}
	
	//전환 조건 트리거 초기화
	handle_animation_transition_trigger_.Unbind();
	handle_animation_transition_trigger_.BindLambda([this](UAZAnimInstance* T) -> bool {return false;});
}

void UAZAnimInstance_Player::SetDirectAnimSequence(FName sequence_name)
{
	is_montage_ = false;
	should_transition_ = true;
	next_sequence_name_= sequence_name;
}
void UAZAnimInstance_Player::SetDirectAnimMontage(FName montage_name)
{
	is_montage_ = true;
	should_transition_ = true;
	next_montage_name_ = montage_name;
	
	//Base(Move,Dash,Slide,Jump)
	//Crouch
	//Hit
	//Airborne
	//WP00 ~ WP11
	//Interaction

	//Grapple
	//Swing
	//Climb
	//Carry

	//Swim
	//Fishing
}
void UAZAnimInstance_Player::SetDirectSection(FName section_name)
{
	is_montage_ = true;
	should_transition_ = true;
	next_section_name_ = section_name;
}

void UAZAnimInstance_Player::SetNextAnimMontage(FName montage_name)
{
	next_montage_name_ = montage_name;
	is_montage_ = true;
}
void UAZAnimInstance_Player::SetNextSection(FName section_name)
{
	next_section_name_ = section_name;
	is_montage_ = true;
}

void UAZAnimInstance_Player::PauseAnimation()
{
	if(is_montage_)
	{
		Montage_Pause(current_anim_montage_);
	}
	else
	{
		//정지
		SetAnimPlayRate(0);
		//TODO: 이전 스피드 저장하여 사용하기
	}
}

void UAZAnimInstance_Player::ResumeAnimation()
{
	if(is_montage_)
	{
		Montage_Resume(current_anim_montage_);
	}
	else
	{
		SetAnimPlayRate(15000);
		//Default 15000
		//TODO: 이전 스피드 저장하여 사용하기
	}
}

void UAZAnimInstance_Player::SetAnimPlayRate(int32 play_rate)
{
	float play_rate_permyriad;
	
	if(play_rate == 0)
	{
		play_rate_permyriad = 0;
	}
	else
	{
		play_rate_permyriad = play_rate/10000.f;
	}
	
	current_anim_play_rate_ = play_rate_permyriad;
	if(Montage_IsPlaying(current_anim_montage_))
	{
		Montage_SetPlayRate(current_anim_montage_, current_anim_play_rate_);
	}
	//SequencePlayer의 AnimPlayRate는 AnimGraph에서 바인딩되어서 실행
}

void UAZAnimInstance_Player::SetAnimationFromTable(int32 anim_hash_code)
{
	//UAZGameSingleton::instance()->player_asset_mgr_->command_bit_mask_map_

	//Rotate
	//InputAction
}
