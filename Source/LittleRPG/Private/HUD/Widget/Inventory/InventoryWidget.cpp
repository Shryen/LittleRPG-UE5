#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "HUD/Widget/Inventory/InventoryItemWidget.h"

void UInventoryWidget::AddItem(const FInventorySlot& InventorySlot)
{
	if (!InventoryGridPanel || !InventoryItemWidgetClass) return;
	
	UInventoryItemWidget* ItemWidget = CreateWidget<UInventoryItemWidget>(this, InventoryItemWidgetClass);
	if (!ItemWidget) return;
	
	int32 Index = InventoryGridPanel->GetChildrenCount();
	int32 Column = Index % ColumnCount;
	int32 Row = Index / ColumnCount;
	
	ItemWidget->InitSlot(InventorySlot);
	SlotWidgetMap.Add(InventorySlot.SlotID, ItemWidget);
    InventoryGridPanel->AddChildToUniformGrid(ItemWidget, Row, Column);
}

void UInventoryWidget::ClearItems()
{
	if (!InventoryGridPanel)
		return;
		
	InventoryGridPanel->ClearChildren();
	SlotWidgetMap.Empty();
}

void UInventoryWidget::UpdateSlot(const FInventorySlot& InventorySlot)
{
	UInventoryItemWidget** Found = SlotWidgetMap.Find(InventorySlot.SlotID);
	
	if (Found)
	{
		(*Found)->InitSlot(InventorySlot);
	}
	else
	{
		AddItem(InventorySlot); 
	}
}
