// Fill out your copyright notice in the Description page of Project Settings.


#include "AZ_MHW/AnimNotify/AZAnimNotify_ActionStart.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Player.h"

UAZAnimNotify_ActionStart::UAZAnimNotify_ActionStart()
{
}

void UAZAnimNotify_ActionStart::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);
	
	if(auto anim_instance = mesh_comp->GetAnimInstance())
	{
		if(auto player_anim_instance = Cast<UAZAnimInstance_Player>(anim_instance))
		{
			player_anim_instance->is_montage_ = true;
			player_anim_instance->should_transition_ = true;

			player_anim_instance->can_input_control_;
		}	
	}
}

