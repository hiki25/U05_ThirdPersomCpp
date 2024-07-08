#pragma once

#include "CoreMinimal.h"
#include "Actions/CDoAction.h"
#include "CDoAction_WhurlWind.generated.h"

class UParticleSystemComponent;
class UBoxComponent;
class ACharacter;

UCLASS()
class THIRDPERSONCPP_API ACDoAction_WhurlWind : public ACDoAction
{
	GENERATED_BODY()

public:
	ACDoAction_WhurlWind();

protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void DoAction() override;
	virtual void Begin_DoAction() override;
	virtual void End_DoAction() override;

	virtual void Abort() override;

private:
	UFUNCTION()
		void TickDamage();
	UFUNCTION()
		void Finish();
public:
	virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) override;
	virtual void OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) override;

private:
	//���� �����?
	UPROPERTY(EditDefaultsOnly, Category = "WhirlWind")
	float DamageToTime;

	//Ȱ�� �ð�
	UPROPERTY(EditDefaultsOnly, Category = "WhirlWind")
	float ActiveTime;

	//ȸ�� �ӵ�
	UPROPERTY(EditDefaultsOnly, Category = "WhirlWind")
	float AroundSpeed;

	//�� ������
	UPROPERTY(EditDefaultsOnly, Category = "WhirlWind")
	float Radius;
	
	UPROPERTY(VisibleInstanceOnly)
	UParticleSystemComponent* AttachedParticleComp;

private:
	UBoxComponent* BoxComp;
	bool bActivating;

	TArray<ACharacter*> HittedCharacters;
	float CurrentYaw;

};
