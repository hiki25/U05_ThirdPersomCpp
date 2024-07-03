#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CActionData.h"
#include "CDoAction.generated.h"

class ACharacter;
class UCAttributeComponent;
class UCStateComponent;

UCLASS()
class THIRDPERSONCPP_API ACDoAction : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDoAction();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
public:
	void SetData(const TArray<FDoActionData>& InDatas);

public:
	virtual void DoAction() {};
	virtual void Begin_DoAction() {};
	virtual void End_DoAction() {};

	virtual void Begin_SubAction() {};
	virtual void End_SubAction() {};

public:
	UFUNCTION()
		virtual void OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) {};
	UFUNCTION()
		virtual void OnAttachmentEndOverlap(ACharacter* InAttacker, AActor* InCauser, ACharacter* InOtherCharacter) {};


protected:
	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;
	UPROPERTY(BlueprintReadOnly)
	UCAttributeComponent* AttributeComponent;
	UPROPERTY(BlueprintReadOnly)
		UCStateComponent* StateComponent;


protected:
	TArray<FDoActionData> Datas;
};
