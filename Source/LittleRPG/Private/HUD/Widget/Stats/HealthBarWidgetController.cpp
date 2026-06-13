#include "HUD/Widget/Stats/HealthBarWidgetController.h"

#include "Component/StatComponent/LittleStatComponent.h"
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
	if (!StatComponent) return;

	StatComponent->OnStatChanged.AddDynamic(this, &UHealthBarWidgetController::OnHealthChanged);

	UpdateHealthBar(StatComponent->GetCurrentHealth(), StatComponent->GetMaxHealth());
}

void UHealthBarWidgetController::OnHealthChanged(EAttribute StatType, float NewValue)
{
	switch (StatType)
	{
		case EAttribute::Health:
			UpdateHealthBar(NewValue, StatComponent->GetMaxHealth());
			break;
		case EAttribute::MaxHealth:
			UpdateHealthBar(StatComponent->GetCurrentHealth(), NewValue);
			break;
		default:
			UpdateHealthBar(StatComponent->GetCurrentHealth(), StatComponent->GetMaxHealth());
		break;
	}
}
