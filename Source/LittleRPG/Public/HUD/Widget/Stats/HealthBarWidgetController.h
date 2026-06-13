#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "HealthBarWidgetController.generated.h"

enum class EAttribute : uint8;
class UHealthWidget;
/**
 * 
 */
UCLASS()
class LITTLERPG_API UHealthBarWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	
	UFUNCTION(BlueprintCallable)
	void UpdateHealthBar(float CurrentHealth, float MaxHealth);
	
	void BindToStatComponent();
	
	UFUNCTION()
	void OnHealthChanged(EAttribute StatType,float NewValue);
	
private:
	UPROPERTY()
	TObjectPtr<UHealthWidget> HealthWidget;
};
