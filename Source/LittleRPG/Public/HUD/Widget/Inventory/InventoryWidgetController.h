#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "InventoryWidgetController.generated.h"

class UInventoryItemWidget;
struct FInventorySlot;
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
	
	UFUNCTION()
	void OnInventoryChanged(const FInventorySlot& SlotChanged);

private:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	bool bIsOpen = false;
	
	UPROPERTY()
	TObjectPtr<ALittlePlayerState> LittlePlayerState;
	
	void ShowInventoryWidget();
	void HideInventoryWidget();
	
	void BindDependencies();
};
