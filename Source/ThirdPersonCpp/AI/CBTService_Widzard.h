#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Widzard.generated.h"

UCLASS()
class THIRDPERSONCPP_API UCBTService_Widzard : public UBTService
{
	GENERATED_BODY()
public:
		UCBTService_Widzard();
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
