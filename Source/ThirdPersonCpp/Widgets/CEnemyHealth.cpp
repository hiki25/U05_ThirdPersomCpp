#include "CEnemyHealth.h"
#include "Components/ProgressBar.h"

void UCEnemyHealth::ApplyHealth_Implementation(float InCurrentHealth, float InMaxHealth)
{
	HealthBar->SetPercent(InCurrentHealth / InMaxHealth);
}