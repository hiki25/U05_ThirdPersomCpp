#include "CBehaviorComponent.h"

UCBehaviorComponent::UCBehaviorComponent()
{
}


void UCBehaviorComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

void UCBehaviorComponent::SetBlackboardComp(UBlackboardComponent* InBlackboardComp)
{
	BlackboardComp = InBlackboardComp;
}

bool UCBehaviorComponent::IsWaitMode()
{
	return false;
}

bool UCBehaviorComponent::IsApprochMode()
{
	return false;
}

bool UCBehaviorComponent::IsActionMode()
{
	return false;
}

bool UCBehaviorComponent::IsPatrolMode()
{
	return false;
}

bool UCBehaviorComponent::IsHittedMode()
{
	return false;
}

bool UCBehaviorComponent::IsEscapeMode()
{
	return false;
}

void UCBehaviorComponent::SetWaitMode()
{
}

void UCBehaviorComponent::SetApprochMode()
{
}

void UCBehaviorComponent::SetActionMode()
{
}

void UCBehaviorComponent::SetPatrolMode()
{
}

void UCBehaviorComponent::SetHittedMode()
{
}

void UCBehaviorComponent::SetEscapeMode()
{
}

void UCBehaviorComponent::ChangeType(EBehaviorType InNewType)
{
}

EBehaviorType UCBehaviorComponent::GetType()
{
	return EBehaviorType();
}


void UCBehaviorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

