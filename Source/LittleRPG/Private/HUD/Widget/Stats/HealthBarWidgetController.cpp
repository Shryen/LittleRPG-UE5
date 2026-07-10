#include "HUD/Widget/Stats/HealthBarWidgetController.h"

#include "Components/ProgressBar.h"
#include "HUD/Widget/Stats/HealthWidget.h"

void UHealthBarWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	HealthWidget = Cast<UHealthWidget>(InWidget); // is this cast succeeding?
	if (!HealthWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHealthBarWidgetController::SetWidget: Cast failed!"));
		return;
	}
}

void UHealthBarWidgetController::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (HealthWidget->GetHealthProgressBar())
		HealthWidget->GetHealthProgressBar()->SetPercent(CurrentHealth / MaxHealth);
}

void UHealthBarWidgetController::BindToStatComponent()
{
	// TODO: Change it later for using AttributeSets
}

