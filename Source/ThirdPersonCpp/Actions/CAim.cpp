#include "CAim.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "CHUD.h"

UCAim::UCAim()
{
	CHelpers::GetAsset(&Curve, "/Game/Game/Curve_Camera");
}

void UCAim::BeginPlay(ACharacter* InOwnerCharacter)
{
	OwnerCharacter = InOwnerCharacter;

	SpringArmComp = CHelpers::GetComponent<USpringArmComponent>(OwnerCharacter);
	CameraComp = CHelpers::GetComponent<UCameraComponent>(OwnerCharacter);

	FOnTimelineFloat OnProgressDelegate;
	OnProgressDelegate.BindUFunction(this, "ZoomIn");
	Timeline.AddInterpFloat(Curve, OnProgressDelegate);

	APlayerController* PC = OwnerCharacter->GetWorld()->GetFirstPlayerController();
	CheckNull(PC);

	HUD = PC->GetHUD<ACHUD>();
}

void UCAim::Tick(float DeltaTime)
{
	Timeline.TickTimeline(DeltaTime);
}

void UCAim::On()
{
	CheckFalse(CanAim());
	CheckTrue(bZooming);

	bZooming = true;

	HUD->VisibleAim();

	SpringArmComp->TargetArmLength = 100.f;
	SpringArmComp->SocketOffset = FVector(0, 30, 10);
	SpringArmComp->bEnableCameraLag = false;

	//CameraComp->FieldOfView = 45.0f;
	Timeline.PlayFromStart();
}

void UCAim::Off()
{
	CheckFalse(CanAim());
	CheckFalse(bZooming);

	bZooming = false;

	HUD->InvisibleAim();

	SpringArmComp->TargetArmLength = 200.f;
	SpringArmComp->SocketOffset = FVector::ZeroVector;
	SpringArmComp->bEnableCameraLag = false;

	//CameraComp->FieldOfView = 90.0f;
	Timeline.ReverseFromEnd();
}

void UCAim::ZoomIn(float Output)
{
	CameraComp->FieldOfView = Output;
}
