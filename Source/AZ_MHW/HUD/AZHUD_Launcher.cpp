// Copyright Team AZ. All Rights Reserved.


#include "AZ_MHW/HUD/AZHUD_Launcher.h"
#include "AZ_MHW/Widget/Launcher/AZWidget_Launcher.h"

void AAZHUD_Launcher::BeginPlay()
{
	Super::BeginPlay();

	UAZWidget_Launcher* launcher = OpenScene<UAZWidget_Launcher>(EUIName::AZWidget_Scene_Launcher);
	//if(launcher) launcher->SetIsDisableBackBtnExit(false);
	
}

void AAZHUD_Launcher::BeginDestroy()
{
	Super::BeginDestroy();
}

void AAZHUD_Launcher::EndPlay(const EEndPlayReason::Type end_paly_reason)
{
	Super::EndPlay(end_paly_reason);
}
