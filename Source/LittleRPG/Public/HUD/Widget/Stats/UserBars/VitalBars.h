#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "VitalBars.generated.h"

class UBaseProgressBar;
/**
 * 
 */
UCLASS()
class LITTLERPG_API UVitalBars : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseProgressBar> HealthBarWidget;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBaseProgressBar> StaminaBarWidget;
};
