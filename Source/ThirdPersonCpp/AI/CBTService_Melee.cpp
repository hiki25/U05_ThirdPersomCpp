#include "CBTService_Melee.h"
#include "Global.h"
#include "Characteres/CAIController.h"
#include "Characteres/CEnemy_AI.h"
#include "Characteres/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"
#include "Components/CPatrolComponent.h"


UCBTService_Melee::UCBTService_Melee()
{
	NodeName = "Root_Melee";
}

void UCBTService_Melee::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* AIC = Cast<ACAIController>(OwnerComp.GetOwner());
	CheckNull(AIC);

	UCBehaviorComponent* BehaviorComp = CHelpers::GetComponent<UCBehaviorComponent>(AIC);

	ACEnemy_AI*  EnemyPawn= Cast<ACEnemy_AI>(AIC->GetPawn());
	CheckNull(EnemyPawn);

	UCStateComponent* StateComp = CHelpers::GetComponent<UCStateComponent>(EnemyPawn);
	UCPatrolComponent* PatrolComp = CHelpers::GetComponent<UCPatrolComponent>(EnemyPawn);

	//Is Hitted
	if (StateComp->IsHittedMode())
	{
		BehaviorComp->SetHittedMode();
		return;
	}

	//Try Get PlayerKey from BB
	ACPlayer* Player = BehaviorComp->GetPlayerKey();

	//No Sense
	if (Player == nullptr)
	{
	AIC->ClearFocus(EAIFocusPriority::Gameplay);
		if (PatrolComp->IsPathValid())
		{
			BehaviorComp->SetPatrolMode();
			return;
		}

		BehaviorComp->SetWaitMode();
		return;
	}

	//Sensed Player
	float Distance = EnemyPawn->GetDistanceTo(Player);

	if (Distance < AIC->GetBehaviorRange())
	{
		BehaviorComp->SetActionMode();
		return;
	}

	if (Distance < AIC->GetSightRadius())
	{
		BehaviorComp->SetApprochMode();
	}


}