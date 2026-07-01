#include "HUD/Widget/Crafting/CraftingWidget.h"
#include "HUD/Widget/Crafting/CraftingSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/Crafting/CraftSlot.h"

void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UCraftingWidget::PopulateSlots(const TArray<FCraftSlot>& Slots)
{
	SlotWidgets.Empty();

	if (!CraftingGrid || !CraftingSlotWidgetClass)
		return;

	for (int32 i = 0; i < Slots.Num(); i++)
	{
		UCraftingSlotWidget* SlotWidget = CreateWidget<UCraftingSlotWidget>(this, CraftingSlotWidgetClass);
		if (SlotWidget)
		{
			SlotWidget->UpdateSlot(Slots[i]);
			CraftingGrid->AddChildToUniformGrid(SlotWidget, i / ColumnCount, i % ColumnCount);
			SlotWidgets.Add(SlotWidget);
		}
	}
}
