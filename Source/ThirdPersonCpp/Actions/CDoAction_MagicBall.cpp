#include "CDoAction_MagicBall.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "CAim.h"
#include "CProjectile.h"


void ACDoAction_MagicBall::BeginPlay()
{
	Super::BeginPlay();

	Aim = NewObject<UCAim>();
	Aim->BeginPlay(OwnerCharacter);

	UCActionComponent* ActionComp = CHelpers::GetComponent<UCActionComponent>(OwnerCharacter);
	ActionComp->OnActionTypeChanged.AddDynamic(this, &ACDoAction_MagicBall::AbortByActionTypeChacnged);
}

void ACDoAction_MagicBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Aim->Tick(DeltaTime);
}

void ACDoAction_MagicBall::DoAction()
{

	Super::DoAction();

	if (Aim->CanAim())
	{
		CheckFalse(Aim->IsZooming());
	}
	CheckFalse(Datas.Num() > 0);

	CheckFalse(StateComponent->IsIdleMode());
	StateComponent->SetActionMode();

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);

	Datas[0].bCanMove ? AttributeComponent->SetMove() : AttributeComponent->SetStop();
}

void ACDoAction_MagicBall::Begin_DoAction()
{
	//Spawn projectile
	Super::Begin_DoAction();

	CheckNull(Datas[0].ProjectileClass);


	FVector CamLoc;
	FRotator CamRot;
	OwnerCharacter->GetController()->GetPlayerViewPoint(CamLoc, CamRot);

	FVector HandLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_R");

	FVector SpawnLocation = CamLoc + CamRot.Vector() * ( (HandLocation - CamLoc) | CamRot.Vector() );

	FTransform Transform;
	Transform.SetLocation(SpawnLocation);
	Transform.SetRotation(FQuat(CamRot));



	ACProjectile* ProjectileInstant = GetWorld()->SpawnActorDeferred<ACProjectile>(Datas[0].ProjectileClass, Transform,OwnerCharacter, OwnerCharacter
		,ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	ProjectileInstant->OnProjectileBeginOverlap.AddDynamic(this, &ACDoAction_MagicBall::OnProjectileBeginOverlap);
	ProjectileInstant->FinishSpawning(Transform);

}

void ACDoAction_MagicBall::End_DoAction()
{
	Super::End_DoAction();
	StateComponent->SetIdleMode();
	AttributeComponent->SetMove();
}

void ACDoAction_MagicBall::Begin_SubAction()
{
	Aim->On();
}

void ACDoAction_MagicBall::End_SubAction()
{
	Aim->Off();
}

void ACDoAction_MagicBall::OnProjectileBeginOverlap(FHitResult InHitResult)
{
	//play particle
	if(Datas[0].Effect)
	{
		FTransform EffectTransform = Datas[0].EffectTransform;
		EffectTransform.AddToTranslation(InHitResult.ImpactPoint);
		EffectTransform.SetRotation(FQuat(InHitResult.ImpactNormal.Rotation()));

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Datas[0].Effect, EffectTransform);
	}

	FDamageEvent DamageEvent;
	InHitResult.GetActor()->TakeDamage(Datas[0].Power, DamageEvent,OwnerCharacter->GetController(),this);
}

void ACDoAction_MagicBall::AbortByActionTypeChacnged(EActionType Inprev, EActionType InNewType)
{
	CheckFalse(Aim->CanAim());
	CheckFalse(Aim->IsZooming());
	Aim->Off();
}
