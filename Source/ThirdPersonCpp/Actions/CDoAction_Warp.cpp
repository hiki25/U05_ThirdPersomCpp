#include "CDoAction_Warp.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CapsuleComponent.h"
#include "CAttachment.h"

void ACDoAction_Warp::BeginPlay()
{
	Super::BeginPlay();

	for (const auto& Child : OwnerCharacter->Children)
	{
		if (Child->IsA<ACAttachment>() && Child->GetActorLabel().Contains("Warp"))
		{
			PreviewMesh = CHelpers::GetComponent<USkeletalMeshComponent>(Child);
			break;
		}
	}
}

void ACDoAction_Warp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	PreviewMesh->SetVisibility(false);


	CheckFalse(*bEquipped);

	FVector CurLoc;
	FRotator CurRot;
	if (GetCursorLoactionAndRotation(CurLoc,CurRot))
	{
		PreviewMesh->SetWorldLocation(CurLoc);
		PreviewMesh->SetVisibility(true);
	}

}

void ACDoAction_Warp::DoAction()
{
	Super::DoAction();

	CheckFalse(StateComponent->IsIdleMode());
	StateComponent->SetActionMode();

	FRotator Temp;
	CheckFalse(GetCursorLoactionAndRotation(Location, Temp));

	OwnerCharacter->PlayAnimMontage(Datas[0].AnimMontage, Datas[0].PlayRate, Datas[0].StartSection);
	Datas[0].bCanMove ? AttributeComponent->SetMove() : AttributeComponent->SetStop();
}

void ACDoAction_Warp::Begin_DoAction()
{
	Super::Begin_DoAction();

	FTransform Transform = Datas[0].EffectTransform;
	Transform.AddToTranslation(OwnerCharacter->GetActorLocation());

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),Datas[0].Effect,Transform);

	PreviewMesh->SetVectorParameterValueOnMaterials("Emissive",FVector(0,0,20) );
}

void ACDoAction_Warp::End_DoAction()
{
	Super::End_DoAction();

	Location.Z += OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();

	OwnerCharacter->SetActorLocation(Location);

	StateComponent->SetIdleMode();
	AttributeComponent->SetMove();

	PreviewMesh->SetVectorParameterValueOnMaterials("Emissive", FVector(20, 0, 0));
}

bool ACDoAction_Warp::GetCursorLoactionAndRotation(FVector& OutLocation, FRotator& OutRotation)
{

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(ObjectTypeQuery1);

	FHitResult Hit;

	if (PC->GetHitResultUnderCursorForObjects(ObjectTypes, true, Hit))
	{
		OutLocation = Hit.Location;
		OutRotation = Hit.ImpactNormal.Rotation();

		return true;
	}

	return false;
}
