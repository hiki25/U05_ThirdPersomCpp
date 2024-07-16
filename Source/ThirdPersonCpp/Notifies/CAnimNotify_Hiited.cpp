#include "CAnimNotify_Hiited.h"
#include "Global.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"

FString UCAnimNotify_Hiited::GetNotifyName_Implementation() const
{
	return "Hiited";
}

void UCAnimNotify_Hiited::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	CheckNull(MeshComp->GetOwner());

	UCStateComponent* StateComp = CHelpers::GetComponent<UCStateComponent>(MeshComp->GetOwner());
	CheckNull(StateComp);
	UCAttributeComponent* AttibuteComp = CHelpers::GetComponent<UCAttributeComponent>(MeshComp->GetOwner());
	CheckNull(AttibuteComp);

	StateComp->SetIdleMode();
	AttibuteComp->SetMove();
}
