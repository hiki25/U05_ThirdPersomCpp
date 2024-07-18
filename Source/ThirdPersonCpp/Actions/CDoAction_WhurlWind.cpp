#include "CDoAction_WhurlWind.h"
#include "Global.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/Character.h"
#include "Components/BoxComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "CAttachment.h"

ACDoAction_WhurlWind::ACDoAction_WhurlWind()
{
	DamageToTime = 0.1f;
	ActiveTime = 5.0f;
	AroundSpeed = 300.f;
	Radius = 150.0f;
}

void ACDoAction_WhurlWind::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Child : OwnerCharacter->Children)
	{
		if (Child->IsA<ACAttachment>() && Child->GetActorLabel().Contains("WhirlWind"))
		{
			BoxComp = CHelpers::GetComponent<UBoxComponent>(Child);
			break;
		}
	}

	CheckNull(BoxComp);
	ACAttachment* Attachment = Cast<ACAttachment>(BoxComp->GetOwner());
	CheckNull(Attachment);
	Attachment->OffCollision();

}

void ACDoAction_WhurlWind::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Rotate WhirlWind
	FVector Center = OwnerCharacter->GetActorLocation();

	CurrentYaw += AroundSpeed* DeltaTime;
	CurrentYaw = FMath::Fmod(CurrentYaw,360.0f);

	FVector AwayFrom = FVector(Radius, 0, 0);
	FVector CollisionLocation = AwayFrom.RotateAngleAxis(CurrentYaw,FVector::UpVector);

	CollisionLocation += Center;

	BoxComp->SetWorldLocation(CollisionLocation);


}

void ACDoAction_WhurlWind::DoAction()
{
	//play anim montage
	Super::DoAction();

	CheckFalse(Datas.Num() > 0);
	CheckFalse(StateComponent->IsIdleMode());

	CheckTrue(bActivating);
	StateComponent->SetActionMode();
	bActivating = true;

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? AttributeComponent->SetMove() : AttributeComponent->SetStop();
}

void ACDoAction_WhurlWind::Begin_DoAction()
{
	//Attach particle
	Super::Begin_DoAction();

	CurrentYaw = OwnerCharacter->GetActorForwardVector().Rotation().Yaw;

	CheckNull(BoxComp);
	AttachedParticleComp = UGameplayStatics::SpawnEmitterAttached(Datas[0].Effect, BoxComp, "");
	AttachedParticleComp->SetRelativeTransform(Datas[0].EffectTransform);

	//OnCollision
	ACAttachment* Attachment = Cast<ACAttachment>(BoxComp->GetOwner());
	CheckNull(Attachment);
	Attachment->OnCollision();

	//Damage
	UKismetSystemLibrary::K2_SetTimer(this,"TickDamage",DamageToTime,true);

}

void ACDoAction_WhurlWind::End_DoAction()
{
	Super::End_DoAction();

	StateComponent->SetIdleMode();
	AttributeComponent->SetMove();

	//Finish TickDamage
	FTimerDynamicDelegate FinishDelegate;
	FinishDelegate.BindUFunction(this, "Finish");
	UKismetSystemLibrary::K2_SetTimerDelegate(FinishDelegate,ActiveTime,false);

}

void ACDoAction_WhurlWind::Abort()
{
	Finish();
}

void ACDoAction_WhurlWind::TickDamage()
{
	FDamageEvent DamageEvent;

	for (int32 i = 0; i < HittedCharacters.Num(); i++)
	{
		UCStateComponent* OtherStateComp = CHelpers::GetComponent<UCStateComponent>(HittedCharacters[i]);


		//Check HittedCharacter Empty and State
		bool bIgnore = false;
		bIgnore |= (OtherStateComp->IsDeadMode());
		bIgnore |= (HittedCharacters[i]->IsPendingKill());
		bIgnore |= (HittedCharacters[i] == nullptr);

		if (bIgnore)
		{
			continue;
		}

		//TakeDamage
		HittedCharacters[i]->TakeDamage(Datas[0].Power, DamageEvent,OwnerCharacter->GetController(),this);
	}
}

void ACDoAction_WhurlWind::Finish()
{

	bActivating = false;

	//Destory Particle
	if(AttachedParticleComp)
	{
		AttachedParticleComp->DestroyComponent();
	}

	//OffCollision
	ACAttachment* Attachment = Cast<ACAttachment>(BoxComp->GetOwner());
	if (Attachment)
	{
	Attachment->OffCollision();
	}

	//Stop Timer
	UKismetSystemLibrary::K2_ClearTimer(this,"TickDamage");

}


//Add HiitedCharacter 
void ACDoAction_WhurlWind::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentBeginOverlap(InAttacker,InCauser,InOtherCharacter);
	HittedCharacters.AddUnique(InOtherCharacter);
}

void ACDoAction_WhurlWind::OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter)
{
	Super::OnAttachmentEndOverlap(InAttacker, InCauser, InOtherCharacter);
	HittedCharacters.Remove(InOtherCharacter);
}
