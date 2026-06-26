#include "HUD/Widget/Inventory/InventoryItemWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/FInventorySlot.h"
#include "Data/ItemDataRow.h"
#include "Engine/DataTable.h"

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
 
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnItemClicked.Broadcast(CachedSlotID);
        UE_LOG(LogTemp, Warning, TEXT("Item was clicked [%d]"), CachedSlotID)
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UInventoryItemWidget::SetItemRow(const FItemDataRow* Row, int32 Quantity)
{
	if (!Row) return;
	
	if (!ItemIcon || !ItemQuantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItemWidget::SetItemData: No image or text block set for widget!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Loading icon: %s"), *Row->ItemIcon.ToSoftObjectPath().ToString());

	if (ItemQuantity)
		ItemQuantity->SetText(FText::AsNumber(Quantity));

	if (ItemIcon && !Row->ItemIcon.IsNull())
		ItemIcon->SetBrushFromTexture(Row->ItemIcon.LoadSynchronous());
}

void UInventoryItemWidget::UpdateQuantity(const FInventorySlot& InventorySlot)
{
	if (!ItemQuantity)
		return;
	
	if (InventorySlot.Quantity <= 0)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	if (InventorySlot.Quantity <= 1)
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
		return;
	}
	
	ItemQuantity->SetVisibility(ESlateVisibility::Visible); 
	ItemQuantity->SetText(FText::AsNumber(InventorySlot.Quantity));
}

void UInventoryItemWidget::InitSlot(const int32 SlotIndex)
{
	CachedSlotID = SlotIndex;
}

void UInventoryItemWidget::UpdateSlot(const FInventorySlot& InventorySlot)
{
	const FItemDataRow* Row = nullptr;
	if (ItemDataTable)
	{
		Row = ItemDataTable->FindRow<FItemDataRow>(InventorySlot.ItemRowName, "");
	}
	SetItemRow(Row, InventorySlot.Quantity);
	UpdateQuantity(InventorySlot);
}

