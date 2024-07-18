#include "CBTService_Widzard.h"
#include "Global.h"
#include "Characteres/CAIController.h"
#include "Characteres/CEnemy_AI.h"
#include "Characteres/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

UCBTService_Widzard::UCBTService_Widzard()
{
	NodeName = "Root_Wizard";
}

void UCBTService_Widzard::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ACAIController* AIC = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(AIC);

	UCBehaviorComponent* BehaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(AIC);

	ACEnemy_AI* EnemyPawn = Cast<ACEnemy_AI>(AIC->GetPawn());
	CheckNull(EnemyPawn);

	UCStateComponent* StateComp = CHelpers::GetComponent<UCStateComponent>(EnemyPawn);

	//Is Hitted
	if (StateComp->IsHittedMode())
	{
		BehaviorComp->SetHittedMode();
		return;
	}

	ACPlayer* Player = BehaviorComp->GetPlayerKey();

	if (Player == nullptr)
	{
		BehaviorComp->SetWaitMode();
		AIC->ClearFocus(EAIFocusPriority::Gameplay);

		return;
	}

	//Sense Player
	AIC->SetFocus(Player);

	float Distance = EnemyPawn->GetDistanceTo(Player);

	//Player in Escape Zone
	if (Distance < AIC->GetBehaviorRange())
	{
		BehaviorComp->SetEscapeMode();
		return;
	}

	//Player in Sight Zone
	if (Distance < AIC->GetSightRadius())
	{
		BehaviorComp->SetActionMode();
		return;
	}
}
