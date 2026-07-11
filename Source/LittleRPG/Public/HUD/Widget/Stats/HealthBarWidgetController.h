#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "HealthBarWidgetController.generated.h"

class UHealthWidget;
struct FOnAttributeChangeData;

UCLASS()
class LITTLERPG_API UHealthBarWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	
	void BindToHealthAttribute();
	
private:
	UPROPERTY()
	TObjectPtr<UHealthWidget> HealthWidget;

	void OnHealthChanged(const FOnAttributeChangeData& Data);

	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
};
