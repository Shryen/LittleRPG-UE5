#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "ProgressBarWidgetController.generated.h"

class UBaseProgressBar;
struct FOnAttributeChangeData;

UCLASS()
class LITTLERPG_API UProgressBarWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LittleRPG|ProgressBar")
	FGameplayAttribute CurrentAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LittleRPG|ProgressBar")
	FGameplayAttribute MaxAttribute;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LittleRPG|ProgressBar")
	float FixedMaxValue = 1.f;

	void BindToAttribute();

private:
	UPROPERTY()
	TObjectPtr<UBaseProgressBar> ProgressBarWidget;

	float CachedCurrent = 0.f;
	float CachedMax = 0.f;

	void OnCurrentChanged(const FOnAttributeChangeData& Data);
	void OnMaxChanged(const FOnAttributeChangeData& Data);
	void Refresh() const;
};
