// Copyright Team AZ. All Rights Reserved.


#include "AZAnimNotify_ChangeSocket.h"
#include "AZ_MHW/Actor/AZSocketActor.h"
#include "AZ_MHW/Character/Player/AZPlayer.h"

UAZAnimNotify_ChangeSocket::UAZAnimNotify_ChangeSocket()
{
}

void UAZAnimNotify_ChangeSocket::Notify(USkeletalMeshComponent* mesh_comp, UAnimSequenceBase* animation,
	const FAnimNotifyEventReference& event_reference)
{
	Super::Notify(mesh_comp, animation, event_reference);

	if(auto player = Cast<AAZPlayer>(mesh_comp->GetOwner()))
	{
		if(auto socket_actor = player->character_sockets_map_.Find(socket_actor_name_))
		{
			(*socket_actor)->SetSocketComponent(to_socket_name_);
		}
	}
}
