#include "HUD/Widget/Stats/ProgressBarWidgetController.h"

#include "AbilitySystem/AttributeSet/LittleAttributeSet.h"
#include "HUD/Widget/Stats/BaseProgressBar.h"

void UProgressBarWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	ProgressBarWidget = Cast<UBaseProgressBar>(InWidget);
	if (!ProgressBarWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UProgressBarWidgetController::SetWidget: Cast to UBaseProgressBar failed!"));
		return;
	}
}

void UProgressBarWidgetController::BindToAttribute()
{
	if (!AbilitySystemComponent || !ProgressBarWidget) return;

	if (CurrentAttribute.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(CurrentAttribute)
			.AddUObject(this, &ThisClass::OnCurrentChanged);

		CachedCurrent = AbilitySystemComponent->GetNumericAttribute(CurrentAttribute);
	}

	if (MaxAttribute.IsValid())
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MaxAttribute)
			.AddUObject(this, &ThisClass::OnMaxChanged);

		CachedMax = AbilitySystemComponent->GetNumericAttribute(MaxAttribute);
	}
	else
	{
		CachedMax = FixedMaxValue;
	}

	Refresh();
}

void UProgressBarWidgetController::OnCurrentChanged(const FOnAttributeChangeData& Data)
{
	CachedCurrent = Data.NewValue;
	Refresh();
}

void UProgressBarWidgetController::OnMaxChanged(const FOnAttributeChangeData& Data)
{
	CachedMax = Data.NewValue;
	Refresh();
}

void UProgressBarWidgetController::Refresh() const
{
	ProgressBarWidget->UpdateProgressBar(CachedCurrent, CachedMax);
}
