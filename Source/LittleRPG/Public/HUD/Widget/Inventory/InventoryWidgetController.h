#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "InventoryWidgetController.generated.h"


class ALittlePlayerState;
class UInventoryWidget;

UCLASS()
class LITTLERPG_API UInventoryWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();
	
	void BindPlayerStateToInventory(ALittlePlayerState* PS);

private:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	bool bIsOpen = false;
	
	void ShowInventoryWidget();
	void HideInventoryWidget();
	void PopulateInventory() const;
	
};
