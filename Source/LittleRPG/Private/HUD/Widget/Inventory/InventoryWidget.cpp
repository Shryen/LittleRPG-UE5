#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/FInventorySlot.h"
#include "HUD/Widget/Inventory/InventoryItemWidget.h"


void UInventoryWidget::UpdateSlot(const FInventorySlot& InSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::UpdateSlot: trying to update: InSlot"));
	if (UInventoryItemWidget** Widget = SlotWidgets.Find(InSlot.SlotID))
	{
		(*Widget)->UpdateSlot(InSlot);
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidget::UpdateSlot: Slot changed"));
	}
}

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (InventoryScrollBox || !InventoryGridPanel)
		return;

	const int32 MaxSlots = 30; // or from player data

	for (int32 i = 0; i < MaxSlots; i++)
	{
		UInventoryItemWidget* SlotWidget =
			CreateWidget<UInventoryItemWidget>(GetWorld(), InventoryItemWidgetClass);

		SlotWidget->ItemDataTable = ItemDataTable;
		SlotWidget->InitSlot(i);

		InventoryGridPanel->AddChildToUniformGrid(SlotWidget, i / 6, i % 6);

		SlotWidgets.Add(i, SlotWidget);
	}
}
