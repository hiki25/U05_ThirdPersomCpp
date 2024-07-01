#include "CDoAction_Melee.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"


void ACDoAction_Melee::DoAction()
{
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);
	if (bCanCombo)
	{
		bCanCombo = false;
		bSuccessCombo = true;

		return;
	}


	CheckFalse(StateComponent->IsIdleMode());
	StateComponent->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage,Datas[0].PlayRate,Datas[0].StartSection);
	Datas[0].bCanMove ? AttributeComponent->SetMove() : AttributeComponent->SetStop();
}

//Play Next Action
void ACDoAction_Melee::Begin_DoAction()
{
	Super::Begin_DoAction();

	CheckFalse(bSuccessCombo);
	bSuccessCombo = false;

	OwnerCharacter->StopAnimMontage();

	ComboCount++;
	ComboCount = FMath::Clamp(ComboCount, 0, Datas.Num() - 1);

	OwnerCharacter->PlayAnimMontage(Datas[ComboCount].AnimMontage, Datas[ComboCount].PlayRate, Datas[ComboCount].StartSection);
	Datas[ComboCount].bCanMove ? AttributeComponent->SetMove() : AttributeComponent->SetStop();
}

void ACDoAction_Melee::End_DoAction()
{
	Super::End_DoAction();

	OwnerCharacter->StopAnimMontage();
	ComboCount = 0;

	StateComponent->SetIdleMode();
	AttributeComponent->SetMove();
}

void ACDoAction_Melee::EnableCombo()
{
	bCanCombo = true;
}

void ACDoAction_Melee::DisableCombo()
{
	bCanCombo = false;
}

void ACDoAction_Melee::ClearHittedCharacter()
{
	HittedCharacters.Empty();
}

void ACDoAction_Melee::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InCauser, InOtherCharacter);
	
	//Blocking Multipe Hit
	int32 NumberOfHittedCharacters = HittedCharacters.Num();
	HittedCharacters.AddUnique(InOtherCharacter);
	CheckFalse(NumberOfHittedCharacters < HittedCharacters.Num());

	//Hit Stop
	float HitStop = Datas[ComboCount].HitStop;

	if (FMath::IsNearlyZero(HitStop) == false)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.02f);
		UKismetSystemLibrary::K2_SetTimer(this, "RestoreGlobalTimeDilation", HitStop * 0.02f, false);
	}


	//CameraShake
	TSubclassOf<UCameraShake> ShakeClass = Datas[ComboCount].ShakeClass;
	if (ShakeClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			PC->PlayerCameraManager->PlayCameraShake(ShakeClass);
		}
	}

	//TakeDamage
	FDamageEvent DamageEvent;
	InOtherCharacter->TakeDamage(Datas[ComboCount].Power, DamageEvent,InAttacker->GetController(),InCauser);
}

void ACDoAction_Melee::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InCauser, InOtherCharacter);
}
