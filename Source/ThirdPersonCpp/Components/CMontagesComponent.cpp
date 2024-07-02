#include "CMontagesComponent.h"
#include "GameFramework/Character.h"
#include "Global.h"

UCMontagesComponent::UCMontagesComponent()
{
}


void UCMontagesComponent::BeginPlay()
{
	Super::BeginPlay();

	if (DataTable == nullptr)
	{
		CLog::Log("DataTable is not Set");
		return;
	}

	TArray<FMontageData*> FromDatasTable;
	DataTable->GetAllRows<FMontageData>("", FromDatasTable);

	for (int32 i = 0; i < (int32)EStateType::Max; i++)
	{
		for (const auto& Data : FromDatasTable)
		{
			if ((EStateType)i == Data->Type)
			{
				Datas[i] = Data;
				break;
			}
		}
	}

}

void UCMontagesComponent::PlayRoll()
{
	PlayAnimMontage(EStateType::Roll);
}

void UCMontagesComponent::PlayBackStep()
{
	PlayAnimMontage(EStateType::Backstep);
}

void UCMontagesComponent::PlayHitted()
{
	PlayAnimMontage(EStateType::Hitted);
}

void UCMontagesComponent::PlayDead()
{
	PlayAnimMontage(EStateType::Dead);
}

void UCMontagesComponent::PlayAnimMontage(EStateType InStateType)
{
	ACharacter* Character = Cast<ACharacter>(GetOwner());
	CheckNull(Character);


	const FMontageData* Data = Datas[(int32)InStateType];
	if (Data && Data->AnimMontage)
	{
			Character->PlayAnimMontage(Data->AnimMontage, Data->PlayRate, Data->StartSection);	
	}
}