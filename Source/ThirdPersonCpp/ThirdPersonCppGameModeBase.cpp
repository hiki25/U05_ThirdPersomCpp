#include "ThirdPersonCppGameModeBase.h"
#include "Global.h"

AThirdPersonCppGameModeBase::AThirdPersonCppGameModeBase()
{
	CHelpers::GetClass<APawn>(&DefaultPawnClass, "Blueprint'/Game/Character/Player/BP_CPlayer.BP_CPlayer_C'");
}
