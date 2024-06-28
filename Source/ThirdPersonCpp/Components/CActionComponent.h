#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CActionComponent.generated.h"

class UCActionData;



UENUM(BlueprintType)
enum class EActionType : uint8
{
	UnArmed,Fist,OneHand,TwoHand, MagicBall, Warp, WhirlWind,Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FActionTypeChanged, EActionType, InPrevType, EActionType, InNewType);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class THIRDPERSONCPP_API UCActionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCActionComponent();

protected:
	virtual void BeginPlay() override;


public:
	void DoAction();

public:
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsUnArmedMode() { return Type == EActionType::UnArmed; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsOneHandMode() { return Type == EActionType::OneHand; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsTwoHandMode() { return Type == EActionType::TwoHand; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsMagicBallMode() { return Type == EActionType::MagicBall; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsWarpMode() { return Type == EActionType::Warp; }

	UFUNCTION(BlueprintPure)
	FORCEINLINE bool IsWhirlWindMode() { return Type == EActionType::WhirlWind; }

public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE UCActionData* GetCurrentActionData() { return DataAssets[(int32)Type]; }

public:
	void SetUnArmedMode();
	void SetFistMode();
	void SetOneHandMode();
	void SetTwoHandMode();
	void SetMagicBallMode();
	void SetWarpMode();
	void SetWhirlWindMode();

private:
	void SetMode(EActionType InNewType);
	void ChangeType(EActionType InNewType);

public:
	UPROPERTY(BlueprintAssignable)
	FActionTypeChanged OnActionTypeChanged;

private:
	UPROPERTY(EditDefaultsOnly, Category = "DataAsset")
	UCActionData* DataAssets[(int32)EActionType::Max];

private:
	EActionType Type;


		
};
