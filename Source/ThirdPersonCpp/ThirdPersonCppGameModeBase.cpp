#include "ThirdPersonCppGameModeBase.h"
#include "Global.h"
#include "CHUD.h"


AThirdPersonCppGameModeBase::AThirdPersonCppGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Character/Player/BP_CPlayer.BP_CPlayer_C'");

	CHelpers::GetClass<AHUD>(&HUDClass, "Blueprint'/Game/Game/BP_HUD.BP_HUD_C'");
}


