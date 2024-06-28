#include "CStateComponent.h"

UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UCStateComponent::SetIdleMode()
{
	ChahgeType(EStateType::Idle);
}

void UCStateComponent::SetRollMode()
{
	ChahgeType(EStateType::Roll);
}

void UCStateComponent::SetBackstepMode()
{
	ChahgeType(EStateType::Backstep);
}

void UCStateComponent::SetEquipMode()
{
	ChahgeType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChahgeType(EStateType::Action);
}

void UCStateComponent::SetHittedMode()
{
	ChahgeType(EStateType::Hitted);
}

void UCStateComponent::SetDeadMode()
{
	ChahgeType(EStateType::Dead);
}

void UCStateComponent::ChahgeType(EStateType InNewType)
{
	if (OnStateTypeChanged.IsBound())
	{
		EStateType Prev = Type;
		Type = InNewType;

		OnStateTypeChanged.Broadcast(Prev, InNewType);
	}
}



