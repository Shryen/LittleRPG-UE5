#include "HUD/Widget/Stats/HealthWidget.h"

#include "Components/ProgressBar.h"

void UHealthWidget::UpdateHealth(float CurrentHealth, float MaxHealth)
{
	if (HealthBar)
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
}
