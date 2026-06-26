#include "HUD/Widget/Inventory/Equipment/EquipmentPanelWidget.h"

#include "Components/VerticalBox.h"
#include "Data/EquipmentSlot.h"
#include "HUD/Widget/Inventory/Equipment/EquipmentSlotWidget.h"

void UEquipmentPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!SlotContainer || !SlotWidgetClass)
		return;

	/*
	for (uint8 i = 0; i < (uint8)EEquipmentSlot::Count; i++)
	{
		UEquipmentSlotWidget* SlotWidget = CreateWidget<UEquipmentSlotWidget>(this, SlotWidgetClass);
		if (!SlotWidget) continue;

		SlotWidget->SlotType = (EEquipmentSlot)i;
		SlotWidget->ItemDataTable = ItemDataTable;

		SlotContainer->AddChildToVerticalBox(SlotWidget);
		SlotWidgets.Add(SlotWidget);
	}
	*/
}

void UEquipmentPanelWidget::UpdateSlot(EEquipmentSlot SlotType, FName ItemRowName)
{
	int32 Index = (int32)SlotType;
	if (!SlotWidgets.IsValidIndex(Index))
		return;

	SlotWidgets[Index]->SetItem(ItemRowName);
}

void UEquipmentPanelWidget::ClearAllSlots()
{
	for (UEquipmentSlotWidget* SlotWidget : SlotWidgets)
	{
		if (SlotWidget)
			SlotWidget->ClearSlot();
	}
}
