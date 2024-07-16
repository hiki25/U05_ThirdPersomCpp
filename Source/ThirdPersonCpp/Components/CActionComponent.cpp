#include "CActionComponent.h"
#include "Global.h"
#include "GameFramework/Character.h"
#include "Actions/CActionData.h"
#include "Actions/CEquipment.h"
#include "Actions/CDoAction.h"
#include "Actions/CAttachment.h"
#include "Actions/CAction.h"

UCActionComponent::UCActionComponent()
{

}


void UCActionComponent::BeginPlay()
{
	Super::BeginPlay();

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	CheckNull(OwnerCharacter);

	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (DataAssets[i])
		{
			DataAssets[i]->BeginPlay(OwnerCharacter,&Datas[i]);
		}
		
	}
	
}

void UCActionComponent::DoAction()
{
	CheckTrue(IsUnArmedMode());

	if (Datas[(int32)Type] && Datas[(int32)Type]->GetDoAction())
	{
		ACDoAction* DoAction = Datas[(int32)Type]->GetDoAction();
		DoAction->DoAction();
	}
}

void UCActionComponent::DoSubAction(bool bBegin)
{
	CheckTrue(IsUnArmedMode());

	if (Datas[(int32)Type] && Datas[(int32)Type]->GetDoAction())
	{
		ACDoAction* DoAction = Datas[(int32)Type]->GetDoAction();
		bBegin ? DoAction->Begin_SubAction() : DoAction->End_SubAction();
	}
}

void UCActionComponent::OffAllCollision()
{
	for (const auto& DataAsset : Datas)
	{
		if (DataAsset && DataAsset->GetAttachment())
		{
			DataAsset->GetAttachment()->OffCollision();
		}
	}
}

void UCActionComponent::DestoryAll()
{
	for (int32 i = 0; i < (int32)EActionType::Max; i++)
	{
		if (Datas[i])
		{
			if (Datas[i]->GetDoAction())
			{
				Datas[i]->GetDoAction()->Destroy();
			}
			if (Datas[i]->GetEquipment())
			{
				Datas[i]->GetEquipment()->Destroy();
			}
			if(Datas[i]->GetAttachment())
			{
				Datas[i]->GetAttachment()->Destroy();
			}
		}
	}
}

void UCActionComponent::Abort()
{
	CheckNull(GetCurrentActionData());
	CheckTrue(IsUnArmedMode());

	if (GetCurrentActionData()->GetEquipment())
	{
		GetCurrentActionData()->GetEquipment()->Begin_Equip();
		GetCurrentActionData()->GetEquipment()->End_Equip();
	}

	if (GetCurrentActionData()->GetDoAction())
	{
		GetCurrentActionData()->GetDoAction()->Abort();
	}
}

void UCActionComponent::SetUnArmedMode()
{
	if(Datas[(int32)Type] && Datas[(int32)Type]->GetEquipment())
	{
		Datas[(int32)Type]->GetEquipment()->UnEquip();
	}

	if (Datas[(int32)EActionType::UnArmed]&& Datas[(int32)EActionType::UnArmed]->GetEquipment())
	{
		Datas[(int32)EActionType::UnArmed]->GetEquipment()->Equip();
	}

	ChangeType(EActionType::UnArmed);
}

void UCActionComponent::SetFistMode()
{
	SetMode(EActionType::Fist);
}

void UCActionComponent::SetOneHandMode()
{
	SetMode(EActionType::OneHand);
}

void UCActionComponent::SetTwoHandMode()
{
	SetMode(EActionType::TwoHand);
}

void UCActionComponent::SetMagicBallMode()
{
	SetMode(EActionType::MagicBall);
}

void UCActionComponent::SetWarpMode()
{
	SetMode(EActionType::Warp);
}

void UCActionComponent::SetWhirlWindMode()
{
	SetMode(EActionType::WhirlWind);
}

void UCActionComponent::SetMode(EActionType InNewType)
{
	if (Type == InNewType)
	{
		SetUnArmedMode();
		return;
	}

	else if (IsUnArmedMode() == false)
	{
		if (Datas[(int32)Type] && Datas[(int32)Type]->GetEquipment())
		{
			Datas[(int32)Type]->GetEquipment()->UnEquip();
		}
	}
	if (Datas[(int32)InNewType] && Datas[(int32)InNewType]->GetEquipment())
	{
		Datas[(int32)InNewType]->GetEquipment()->Equip();
	}

	ChangeType(InNewType);
}

void UCActionComponent::ChangeType(EActionType InNewType)
{
	EActionType Prev = Type;
	Type = InNewType;

	

	if (OnActionTypeChanged.IsBound())
	{
		OnActionTypeChanged.Broadcast(Prev, InNewType);
	}
}
