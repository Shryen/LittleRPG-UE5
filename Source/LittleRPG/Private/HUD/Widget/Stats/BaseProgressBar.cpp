#include "HUD/Widget/Stats/BaseProgressBar.h"
#include "Components/ProgressBar.h"

void UBaseProgressBar::UpdateProgressBar(float Value, float MaxValue) const
{
	const float Max = (MaxValue > 0.f) ? MaxValue : 1.f;
	ProgressBar->SetPercent(FMath::Clamp(Value / Max, 0.f, 1.f));
}
