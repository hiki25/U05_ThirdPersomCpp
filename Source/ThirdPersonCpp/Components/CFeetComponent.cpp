#include "CFeetComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"


UCFeetComponent::UCFeetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	LeftFootSocketName = "Foot_L";
	RightFootSocketName = "Foot_R";

	Additional = 55.f;
	FootHeight = 7.5f;
	InterpSpeed = 16.f;

	DrawDebugType = EDrawDebugTrace::ForOneFrame;
}


void UCFeetComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	CapsulHalfHeight = OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	
}


void UCFeetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float LeftDistance;
	FRotator LeftRotation;
	Trace(LeftFootSocketName, LeftDistance, LeftRotation);

	float RightDistance;
	FRotator RightRotation;
	Trace(RightFootSocketName, RightDistance, RightRotation);

	float Floating = FMath::Min(LeftDistance, RightDistance);

	Data.PevisDistance.Z = UKismetMathLibrary::FInterpTo(Data.PevisDistance.Z, Floating,DeltaTime,InterpSpeed);
	//Bone Space
	//Lerp
	Data.LeftDistance.X = UKismetMathLibrary::FInterpTo(Data.LeftDistance.X, (LeftDistance- Floating),DeltaTime,InterpSpeed);
	Data.RightDistance.X = UKismetMathLibrary::FInterpTo(Data.RightDistance.X, -(RightDistance - Floating),DeltaTime,InterpSpeed);


}

void UCFeetComponent::Trace(FName InSocketName, float& OutDistance, FRotator& OutRotation)
{
	OutDistance = 0.f;

	FVector SocketLocation = OwnerCharacter->GetMesh()->GetSocketLocation(InSocketName);
	FVector Start = FVector(SocketLocation.X, SocketLocation.Y, OwnerCharacter->GetActorLocation().Z);

	float TraceZ = Start.Z - CapsulHalfHeight - Additional;
	FVector End = FVector(SocketLocation.X, SocketLocation.Y,TraceZ);


	TArray<AActor*>Ignore;
	Ignore.Add(OwnerCharacter);

	FHitResult HitResult;

	UKismetSystemLibrary::LineTraceSingle
	(GetWorld(),
		Start,
		End,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		true,
		Ignore,
		DrawDebugType,
		HitResult,
		true
		);

	CheckFalse(HitResult.bBlockingHit);

	float Length = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
  	OutDistance = FootHeight + Length - Additional;

	//Todo
	FVector ImpactNormal = HitResult.ImpactNormal;
	OutRotation = ImpactNormal.Rotation();
}

