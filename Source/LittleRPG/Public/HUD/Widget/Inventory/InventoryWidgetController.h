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
	void SetWidget(UInventoryWidget* InWidget);
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
	
	void BindPlayerStateToInventory(APlayerState* InPlayerState);

private:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	bool bIsOpen = false;
	
	void ShowInventoryWidget();
	void HideInventoryWidget();
	void PopulateInventory() const;
	
};
