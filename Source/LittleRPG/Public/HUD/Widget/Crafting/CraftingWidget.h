#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "CraftingWidget.generated.h"

class UCraftingSlotWidget;
class UUniformGridPanel;

DECLARE_MULTICAST_DELEGATE_OneParam(FCraftRequested, FName)

UCLASS()
class LITTLERPG_API UCraftingWidget : public ULittleUserWidget
{
	GENERATED_BODY()

public:
	void PopulateSlots();
	
	FCraftRequested OnCraftRequested;

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> CraftingGrid;

	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Crafting")
	TSubclassOf<UCraftingSlotWidget> CraftingSlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Crafting")
	int32 ColumnCount = 6;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Data")
	TObjectPtr<UDataTable> CraftingData;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Data")
	TObjectPtr<UDataTable> InventoryDataTable;

private:
	UPROPERTY()
	TArray<TObjectPtr<UCraftingSlotWidget>> SlotWidgets;
};
