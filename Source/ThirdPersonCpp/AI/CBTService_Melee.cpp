#include "CBTService_Melee.h"
#include "Global.h"
#include "Characteres/CAIController.h"
#include "Characteres/CEnemy_AI.h"
#include "Characteres/CPlayer.h"
#include "Components/CBehaviorComponent.h"
#include "Components/CStateComponent.h"

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
		BehaviorComp->SetWaitMode();
		return;
	}

	//Sensed Player



}