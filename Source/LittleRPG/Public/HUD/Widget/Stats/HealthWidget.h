#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "HealthWidget.generated.h"

class UProgressBar;

UCLASS()
class LITTLERPG_API UHealthWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateHealth(float CurrentHealth, float MaxHealth);
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UProgressBar> HealthBar;
};
