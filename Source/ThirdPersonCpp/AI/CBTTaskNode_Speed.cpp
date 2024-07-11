#include "CBTTaskNode_Speed.h"
#include "Global.h"
#include "AIController.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";
}

EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIC = Cast<AAIController>(OwnerComp.GetOwner());
	//���ϰ��� �ʿ��� �Լ��̱� ������ NullResult
	CheckNullResult(AIC,EBTNodeResult::Failed);

	APawn* Pawn = AIC->GetPawn();
	CheckNullResult(Pawn, EBTNodeResult::Failed);

	UCAttributeComponent* AttirbuteComp = CHelpers::GetComponent<UCAttributeComponent>(Pawn);
	AttirbuteComp->SetWalkSpeed(Type);

	return EBTNodeResult::Succeeded;
}