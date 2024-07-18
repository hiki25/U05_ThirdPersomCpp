#include "CAnimNotify_BackStepped.h"
#include "Global.h"
#include "Characteres/CPlayer.h"

FString UCAnimNotify_BackStepped::GetNotifyName_Implementation() const
{
	return "BackStepped";
}


void UCAnimNotify_BackStepped::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());

	ACPlayer* Player = Cast<ACPlayer>(MeshComp->GetOwner());
	CheckNull(Player);

	Player->End_Backstep();
}
