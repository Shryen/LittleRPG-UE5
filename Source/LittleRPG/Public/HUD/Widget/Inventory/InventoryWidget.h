#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryWidget.generated.h"


struct FInventorySlot;
class UInventoryItemWidget;
class UItemData;
class UUniformGridPanel;
class UHorizontalBox;

UCLASS()
class LITTLERPG_API UInventoryWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	void AddItem(const FInventorySlot& InventorySlot);
	void ClearItems();
	void UpdateSlot(const FInventorySlot& Slot);


protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> InventoryContainerBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Inventory")
	TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;
	
private:
	int32 ColumnCount = 4;
	UPROPERTY()
	TMap<int32, UInventoryItemWidget*> SlotWidgetMap;
};
