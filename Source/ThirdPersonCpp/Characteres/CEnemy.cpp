#include "CEnemy.h"
#include "Global.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Materials/MaterialInstanceConstant.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CAttributeComponent.h"
#include "Components/CMontagesComponent.h"
#include "Components/CActionComponent.h"
#include "Widgets/CEnenyNameWidget.h"
#include "Widgets/CEnemyHealth.h"
#include "Actions/CActionData.h"

ACEnemy::ACEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	//Property Settings
	LaunchValue = 25;

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

	//GetDissolve CurveAsset
	CHelpers::GetAsset(&DissolveCurve, "/Game/Game/Curve_Dissolve");


}

void ACEnemy::BeginPlay()
{

	//Create Material Dynamic
	{
		UMaterialInstanceConstant* BodyMaterialAsset;
		UMaterialInstanceConstant* LogoMaterialAsset;
		UMaterialInstanceConstant* DissolveMaterialAsset;

		CHelpers::GetAssetDynamic(&BodyMaterialAsset, "/Game/Character/Materials/MI_UE4Man_Body");
		CHelpers::GetAssetDynamic(&LogoMaterialAsset, "/Game/Character/Materials/M_UE4Man_ChestLogo");
		CHelpers::GetAssetDynamic(&DissolveMaterialAsset, "/Game/Materials/MI_Dissovle");

		BodyMaterial = UMaterialInstanceDynamic::Create(BodyMaterialAsset, this);
		LogoMaterial = UMaterialInstanceDynamic::Create(LogoMaterialAsset, this);
		DissolveMaterial = UMaterialInstanceDynamic::Create(DissolveMaterialAsset, this);

		GetMesh()->SetMaterial(0, BodyMaterial);
		GetMesh()->SetMaterial(1, LogoMaterial);
	}

	//Bind StateTypeChange
	StateComp->OnStateTypeChanged.AddDynamic(this, &ACEnemy::OnStateTypeChanged);

	//Begin play
	Super::BeginPlay();

	ActionComp->SetUnArmedMode();


	//Widget
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

	//Bind Dissolve Timeline
	FOnTimelineFloat OnProgressDelegate;
	OnProgressDelegate.BindUFunction(this,"StartDissolve");
	DissolveTime.AddInterpFloat(DissolveCurve, OnProgressDelegate);

	FOnTimelineEvent OnFinishDelegate;
	OnFinishDelegate.BindUFunction(this, "EndDissolve");
	DissolveTime.SetTimelineFinishedFunc(OnFinishDelegate);
	
}

void ACEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DissolveTime.TickTimeline(DeltaTime);
}

void ACEnemy::ChangeBodyColor(FLinearColor InColor)
{
	CheckTrue(StateComp->IsDeadMode());

	if (StateComp->IsHittedMode())
	{
		LogoMaterial->SetScalarParameterValue("bHitted", 1.f);
		LogoMaterial->SetVectorParameterValue("LogoEmissive", InColor);

		return;
	}
	

	BodyMaterial->SetVectorParameterValue("BodyColor", InColor);
	LogoMaterial->SetVectorParameterValue("BodyColor", InColor);
}

float ACEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	DamageValue = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
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
	//Apply Health Widget
	UCEnemyHealth* HealthWidgetInstance = Cast<UCEnemyHealth>(HealthWidgetComp->GetUserWidgetObject());
	if (HealthWidgetInstance)
	{
		HealthWidgetInstance->ApplyHealth(AttributeComp->GetCurrentHealth(), AttributeComp->GetMaxHealth());
	}

	//Hitted Montage
	MontageComp->PlayHitted();

	//Look At Attack
	FVector Start = GetActorLocation();
	FVector Target = DamageInstigator->GetPawn()->GetActorLocation();
	FRotator Rotation = UKismetMathLibrary::FindLookAtRotation(Start, Target);
	SetActorRotation(FRotator(0,Rotation.Yaw,0));

	//Hit Back
	FVector Direction = Start - Target;
	Direction.Normalize();
	float Power = DamageValue * LaunchValue;
	LaunchCharacter(Direction* Power,true,false);

	//Set Hitted Logo Color
	ChangeBodyColor(FLinearColor(40, 0, 0));
	UKismetSystemLibrary::K2_SetTimer(this, "RestoreLogoColor", 0.5f, false);
}

void ACEnemy::Dead()
{
	MontageComp->PlayDead();

	//Widget Hide
	NameWidgetComp->SetVisibility(false);
	HealthWidgetComp->SetVisibility(false);

	//Rag Doll
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	//off All Collision
	ActionComp->OffAllCollision();

	//SetDissolve
	FLinearColor EquipmentColor = FLinearColor::Black;
	if (ActionComp->GetCurrentActionData() && ActionComp->GetCurrentActionData()->GetEquipment())
	{
		EquipmentColor =ActionComp->GetCurrentActionData()->GetEquipmentColor();
	}

	DissolveMaterial->SetVectorParameterValue("BodyColor", EquipmentColor);

	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); i++)
	{
		GetMesh()->SetMaterial(i, DissolveMaterial);
	}

	DissolveTime.PlayFromStart();

}

void ACEnemy::RestoreLogoColor()
{
	LogoMaterial->SetScalarParameterValue("bHitted", 0.f);
}

void ACEnemy::StartDissolve(float InOutput)
{
	CheckNull(DissolveMaterial);

	DissolveMaterial->SetScalarParameterValue("Amount", InOutput);
}

void ACEnemy::EndDissolve()
{
	Destroy();
}

