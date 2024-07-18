#include "CProjectile.h"
#include "Global.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ACProjectile::ACProjectile()
{
	CHelpers::CreateSceneComponent(this, &SphereComp, "SphereComp");
	CHelpers::CreateSceneComponent(this, &ParticleComp, "ParticleComp", SphereComp);

	CHelpers::CreateActorComponent(this, &ProjectileComp, "ProjectileComp");

	InitialLifeSpan = 3.f;
	ProjectileComp->InitialSpeed = 4000.0f;
	ProjectileComp->MaxSpeed = 8000.0f;
	ProjectileComp->ProjectileGravityScale = 0.0f;
}

void ACProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ACProjectile::OnComponentBeginOverlap);
	
}

void ACProjectile::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	CheckTrue(OtherActor == GetOwner());

	if (OnProjectileBeginOverlap.IsBound())
	{
		OnProjectileBeginOverlap.Broadcast(SweepResult);
	}

	Destroy();
}


