#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Components/TimelineComponent.h"
#include "CAim.generated.h"

class ACharacter;
class UCameraComponent;
class USpringArmComponent;
class UCurveFloat;
class ACHUD;

UCLASS()
class THIRDPERSONCPP_API UCAim : public UObject
{
	GENERATED_BODY()

public:
	UCAim();

	void BeginPlay(ACharacter* InOwnerCharacter);
	void Tick(float DeltaTime);

	void On();
	void Off();

	FORCEINLINE bool CanAim() {return SpringArmComp != nullptr && CameraComp != nullptr; }
	FORCEINLINE bool InZooming() { return bZooming; }

private:
	UFUNCTION()
		void ZoomIn(float Output);
private:
	ACharacter* OwnerCharacter;
	UCameraComponent* CameraComp;
	USpringArmComponent* SpringArmComp;

	UCurveFloat* Curve;

	FTimeline Timeline;

	ACHUD* HUD;

	bool bZooming;
};
