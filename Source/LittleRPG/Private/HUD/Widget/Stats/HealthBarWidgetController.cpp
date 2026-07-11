#include "HUD/Widget/Stats/HealthBarWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/LittleAttributeSet.h"
#include "Components/ProgressBar.h"
#include "HUD/Widget/Stats/HealthWidget.h"

void UHealthBarWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	HealthWidget = Cast<UHealthWidget>(InWidget);
	if (!HealthWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UHealthBarWidgetController::SetWidget: Cast failed!"));
		return;
	}
}

void UHealthBarWidgetController::UpdateHealthBar(float CurrentHealth, float MaxHealth)
{
	if (!HealthWidget || !HealthWidget->GetHealthProgressBar()) return;

	const float Percent = (MaxHealth > 0.f) ? (CurrentHealth / MaxHealth) : 0.f;
	HealthWidget->GetHealthProgressBar()->SetPercent(FMath::Clamp(Percent, 0.f, 1.f));
}

void UHealthBarWidgetController::BindToHealthAttribute()
{
	if (!AbilitySystemComponent) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ThisClass::OnHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &ThisClass::OnMaxHealthChanged);

	const float Health = AbilitySystemComponent->GetNumericAttribute(
		ULittleAttributeSet::GetHealthAttribute());
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(
		ULittleAttributeSet::GetMaxHealthAttribute());

	UpdateHealthBar(Health, MaxHealth);
}

void UHealthBarWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	const float MaxHealth = AbilitySystemComponent->GetNumericAttribute(
		ULittleAttributeSet::GetMaxHealthAttribute());
	UpdateHealthBar(Data.NewValue, MaxHealth);
}

void UHealthBarWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	const float Health = AbilitySystemComponent->GetNumericAttribute(
		ULittleAttributeSet::GetHealthAttribute());
	UpdateHealthBar(Health, Data.NewValue);
}
