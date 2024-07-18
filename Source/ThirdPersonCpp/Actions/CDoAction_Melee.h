#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "Actions/CActionData.h"
#include "CDoAction_Melee.generated.h"

UCLASS()
class THIRDPERSONCPP_API ACDoAction_Melee : public ACDoAction
{
	GENERATED_BODY()

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

public:
	void EnableCombo();
	void DisableCombo();
	void ClearHittedCharacter();

private:
	UFUNCTION()
	void RestoreGlobalTimeDilation();

public:
		virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) override;
		virtual void OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) override;

private:
	int32 ComboCount = 0;
	bool bCanCombo;
	bool bSuccessCombo;

	TArray<ACharacter*> HittedCharacters;
};
