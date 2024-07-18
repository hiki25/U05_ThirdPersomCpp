#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CEnemyHealth.generated.h"

class UProgressBar;

UCLASS()
class THIRDPERSONCPP_API UCEnemyHealth : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
		void ApplyHealth(float InCurrentHealth, float InMaxHealth);

protected:
	UPROPERTY(meta = (BindWidget))
		UProgressBar* HealthBar;

};
