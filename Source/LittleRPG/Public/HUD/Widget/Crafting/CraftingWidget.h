#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "CraftingWidget.generated.h"

class UCraftingSlotWidget;
class UUniformGridPanel;

UCLASS()
class LITTLERPG_API UCraftingWidget : public ULittleUserWidget
{
	GENERATED_BODY()

public:
	void PopulateSlots(const TArray<struct FCraftSlot>& Slots);

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UUniformGridPanel> CraftingGrid;

	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Crafting")
	TSubclassOf<UCraftingSlotWidget> CraftingSlotWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Crafting")
	int32 ColumnCount = 3;

private:
	UPROPERTY()
	TArray<TObjectPtr<UCraftingSlotWidget>> SlotWidgets;
};
