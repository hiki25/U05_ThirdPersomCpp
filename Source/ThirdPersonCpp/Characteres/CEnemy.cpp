#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/WidgetComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Widgets/CEnenyNameWidget.h"
#include "Widgets/CEnemyHealth.h"

ACEnemy::ACEnemy()
{

	CHelpers::CreateSceneComponent(this, &NameWidgetComp, "NameWidgetComp", GetMesh());
	CHelpers::CreateSceneComponent(this, &HealthWidgetComp, "HealthWidgetComp", GetMesh());

	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	GetMesh()->SetRelativeLocation(FVector(0, 0, -80));

	USkeletalMesh* MeshAsset;
	CHelpers::GetAsset<USkeletalMesh>(&MeshAsset, "/Game/Character/Mesh/SK_Mannequin");
	GetMesh()->SetSkeletalMesh(MeshAsset);


	//Create Actor Component
	CHelpers::CreateActorComponent(this, &ActionComp, "ActionComp");
	CHelpers::CreateActorComponent(this, &MontageComp, "MontageComp");
	CHelpers::CreateActorComponent(this, &AttributeComp, "AttributeComp");
	CHelpers::CreateActorComponent(this, &StateComp, "StateComp");

	TSubclassOf<UAnimInstance>AnimInstanceClass;
	CHelpers::GetClass<UAnimInstance>(&AnimInstanceClass, "/Game/Enemis/ABP_Enemy");
	GetMesh()->SetAnimInstanceClass(AnimInstanceClass);

	GetCharacterMovement()->MaxWalkSpeed = AttributeComp->GetSprintSpeed();
	GetCharacterMovement()->RotationRate = FRotator(0, 720, 0);

	//Widget
	TSubclassOf<UCEnenyNameWidget> NameWidgetAsset;
	CHelpers::GetClass(&NameWidgetAsset, "/Game/Widgets/WB_EnemyName");
	NameWidgetComp->SetWidgetClass(NameWidgetAsset);
	NameWidgetComp->SetRelativeLocation(FVector(0, 0, 240));
	NameWidgetComp->SetDrawSize(FVector2D(240, 30));
	NameWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);

	TSubclassOf<UCEnemyHealth> HealthWidgetAsset;
	CHelpers::GetClass(&HealthWidgetAsset, "/Game/Widgets/WB_EnemyHealth");
	HealthWidgetComp->SetWidgetClass(HealthWidgetAsset);
	HealthWidgetComp->SetRelativeLocation(FVector(0, 0, 190));
	HealthWidgetComp->SetDrawSize(FVector2D(120, 15));
	HealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);


}

void ACEnemy::BeginPlay()
{

	//Create Material Dynamic
	{
		UMaterialInstanceConstant* BodyMaterialAsset;
		UMaterialInstanceConstant* LogoMaterialAsset;

		CHelpers::GetAssetDynamic(&BodyMaterialAsset, "/Game/Character/Materials/MI_UE4Man_Body");
		CHelpers::GetAssetDynamic(&LogoMaterialAsset, "/Game/Character/Materials/M_UE4Man_ChestLogo");

		BodyMaterial = UMaterialInstanceDynamic::Create(BodyMaterialAsset, this);
		LogoMaterial = UMaterialInstanceDynamic::Create(LogoMaterialAsset, this);

		GetMesh()->SetMaterial(0, BodyMaterial);
		GetMesh()->SetMaterial(1, LogoMaterial);
	}

	StateComp->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	Super::BeginPlay();

	ActionComp->SetUnArmedMode();

	NameWidgetComp->InitWidget();
	UCEnenyNameWidget* NameWidgetInstance = Cast<UCEnenyNameWidget>(NameWidgetComp->GetUserWidgetObject());
	if (NameWidgetInstance)
	{
		NameWidgetInstance->SetName(GetController()->GetName(),GetName());
	}

	HealthWidgetComp->InitWidget();
	UCEnemyHealth* HealthWidgetInstance = Cast<UCEnemyHealth>(HealthWidgetComp->GetUserWidgetObject());
	if (HealthWidgetInstance)
	{
		HealthWidgetInstance->ApplyHealth(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());
	}

	
}

void ACEnemy::ChangeBodyColor(FLinearColor InColor)
{
	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	DamageInstigator = EventInstigator;

	AttributeComp->DecreaseHealth(Damage);

	if (AttributeComp->GetCurrentHealth() <= 0)
	{
		StateComp->SetDeadMode();
		return 0.0f;
	}

	StateComp->SetHittedMode();

	return DamageValue;
}

void ACEnemy::OnStateTypeChanged(EStateType InPrevType, EStateType InNewType)
{
	switch (InNewType)
	{

	case EStateType::Hitted:
	{
		Hitted();
		break;
	}
	case EStateType::Dead:
	{
		Dead();
		break;
	}
	default:
		break;
	}
}

void ACEnemy::Hitted()
{
	UCEnemyHealth* HealthWidgetInstance = Cast<UCEnemyHealth>(HealthWidgetComp->GetUserWidgetObject());
	if (HealthWidgetInstance)
	{
		HealthWidgetInstance->ApplyHealth(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());
	}
}

void ACEnemy::Dead()
{
}

