#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "MainLayoutWidgetController.generated.h"

class UMainLayoutWidget;

UENUM()
enum class EWidgetPage : uint8
{
	Inventory = 0,
	Crafting  = 1,
};

UCLASS()
class LITTLERPG_API UMainLayoutWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	
	void ToggleInventory();

	void SwitchWidget(EWidgetPage Page);
	
private:
	UFUNCTION()
	void SwitchToInventory();
	
	UFUNCTION()
	void SwitchToCrafting();

	UPROPERTY()
	TObjectPtr<UMainLayoutWidget> MainLayoutWidget;
	
	bool bInventoryOpen = false;
	
	void ShowInventoryWidget();
	void HideInventoryWidget();
};
