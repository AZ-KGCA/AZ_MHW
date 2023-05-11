// Copyright Team AZ. All Rights Reserved.


#include "AZAnimNotify_ChangeMontage.h"
#include "AZ_MHW/AnimInstance/AZAnimInstance_Player.h"

UAZAnimNotify_ChangeMontage::UAZAnimNotify_ChangeMontage()
{
	
}

void UAZAnimNotify_ChangeMontage::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
                                       const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if(auto anim_instance = mesh_comp->GetAnimInstance())
	{
		if(auto player_anim_instance = Cast<UAZAnimInstance_Player>(anim_instance))
		{
			player_anim_instance->is_montage_ = true;
			player_anim_instance->should_transition_ = true;
			player_anim_instance->next_montage_name_ = target_montage_name_;
			player_anim_instance->next_section_name_ = target_section_name_;
		}	
	}
}
