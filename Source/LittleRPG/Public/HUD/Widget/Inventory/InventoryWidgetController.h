#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "InventoryWidgetController.generated.h"


class UInventoryWidget;

UCLASS()
class LITTLERPG_API UInventoryWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
	void BindPlayerStateToInventory();

private:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	bool bIsOpen = false;
	
	void ShowInventoryWidget();
	void HideInventoryWidget();
	void PopulateInventory() const;
	
};
