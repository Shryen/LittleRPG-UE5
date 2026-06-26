#include "HUD/Widget/Inventory/InventoryWidget.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Components/UniformGridPanel.h"
#include "Data/Inventory/InventoryDisplayPayLoad.h"
#include "HUD/Widget/Inventory/InventoryItemWidget.h"


void UInventoryWidget::UpdateFromPayload(const FInventoryDisplayPayload& Payload)
{
	UInventoryItemWidget** Cell = SlotWidgets.Find(Payload.VisualSlotIndex);
	if (!Cell || !*Cell)
		return;
 
	(*Cell)->UpdateFromPayload(Payload);
}


void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!InventoryGridPanel)
		return;

	const int32 MaxSlots = ULittleInventoryManagerComponent::MaxVisualSlots;
	
	for (int32 i = 0; i < MaxSlots; i++)
	{
		UInventoryItemWidget* Cell = CreateWidget<UInventoryItemWidget>(this, InventoryItemWidgetClass);
		if (!Cell)
			continue;
 
		Cell->VisualSlotIndex = i;
 
		Cell->SetVisibility(ESlateVisibility::Collapsed);
		
		Cell->OnItemClicked.AddUObject(this, &UInventoryWidget::OnCellRightClicked);
 
		const int32 Row    = i / ColumnCount;
		const int32 Column = i % ColumnCount;
		InventoryGridPanel->AddChildToUniformGrid(Cell, Row, Column);
 
		SlotWidgets.Add(i, Cell);
	}
}

void UInventoryWidget::OnCellRightClicked(int32 VisualSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("[InventoryWidget] OnCellRightClicked VisualSlotIndex=%d"), VisualSlotIndex);
	OnInventoryItemRightClicked.Broadcast(VisualSlotIndex);
}
