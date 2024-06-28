#include "CEquipment.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "Interfaces/CCharacterInterface.h"

ACEquipment::ACEquipment()
{

}

void ACEquipment::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	StateComp = CHelpers::GetComponent<UCStateComponent>(OwnerCharacter);
	AttributeComp = CHelpers::GetComponent<UCAttributeComponent>(OwnerCharacter);
	
}

void ACEquipment::SetData(const FEquipmentData& InData)
{
	Data = InData;
}

void ACEquipment::SetColor(const FLinearColor& InColor)
{
	Color = InColor;
}

void ACEquipment::Equip_Implementation()
{
	StateComp->SetEquipMode();
	if (Data.AnimMontage)
	{
		OwnerCharacter->PlayAnimMontage(Data.AnimMontage, Data.PlayRate, Data.StartSection);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	if (Data.bLookForward)
	{
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		OwnerCharacter->bUseControllerRotationYaw = true;
	}

	(Data.bCanMove ? AttributeComp->SetMove() : AttributeComp->SetStop());

	ICCharacterInterface* CharacterInterface = Cast<ICCharacterInterface>(OwnerCharacter);
	CheckNull(CharacterInterface);

	CharacterInterface->ChangeBodyColor(Color);
}

void ACEquipment::Begin_Equip_Implementation()
{
	if (OnEquipmentDelegate.IsBound())
	{
		OnEquipmentDelegate.Broadcast();
	}
}

void ACEquipment::End_Equip_Implementation()
{
	StateComp->SetIdleMode();
	AttributeComp->SetMove();
}

void ACEquipment::UnEquip_Implementation()
{
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
	OwnerCharacter->bUseControllerRotationYaw = false;

	if (OnUnequipmentDelegate.IsBound())
	{
		OnUnequipmentDelegate.Broadcast();
	}
}

