#include "CBTTaskNode_Action.h"
#include "Global.h"
#include "AIController.h"
#include "Components/CActionComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CBehaviorComponent.h"
#include "Characteres/CPlayer.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";
	Delay = 2.0f;

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* AIC = Cast<AAIController>(OwnerComp.GetOwner());
	//리턴값이 필요한 함수이기 때문에 NullResult
	CheckNullResult(AIC, EBTNodeResult::Failed);

	APawn* Pawn = AIC->GetPawn();
	CheckNullResult(Pawn, EBTNodeResult::Failed);

	AIC->SetFocus(nullptr);

	UCActionComponent* ActionComp = CHelpers::GetComponent<UCActionComponent>(Pawn);
	ActionComp->DoAction();

	ElapsedTime = 0;

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);


	AAIController* AIC = Cast<AAIController>(OwnerComp.GetOwner());
	CheckNull(AIC);

	APawn* Pawn = AIC->GetPawn();
	CheckNull(Pawn);

	UCStateComponent* StateComp = CHelpers::GetComponent<UCStateComponent>(Pawn);

	

	ElapsedTime += DeltaSeconds;
	if (StateComp->IsIdleMode() && ElapsedTime > Delay)
	{
		UCBehaviorComponent* BehaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(AIC);
		AIC->SetFocus(BehaviorComp->GetPlayerKey());
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
