#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryWidget.generated.h"

struct FInventorySlot;
class UInventoryItemWidget;
class UUniformGridPanel;
class UHorizontalBox;
class UScrollBox;

UCLASS()
class LITTLERPG_API UInventoryWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	
	void UpdateSlot(const FInventorySlot& Slot);

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> InventoryContainerBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Inventory")
	TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UScrollBox> InventoryScrollBox;
	
	UPROPERTY()
	TMap<int32, UInventoryItemWidget*> SlotWidgets;
	
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> ItemDataTable;
};
