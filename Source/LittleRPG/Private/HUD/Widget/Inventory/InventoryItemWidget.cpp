#include "HUD/Widget/Inventory/InventoryItemWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/FInventorySlot.h"
#include "Data/ItemData.h"

void UInventoryItemWidget::SetItemData(const UItemData* Item)
{
	if (!Item) return;
	
	if (!ItemIcon || !ItemQuantity)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryItemWidget::SetItemData: No image or text block set for widget!"));
		return;
	}

	if (ItemQuantity)
		ItemQuantity->SetText(FText::AsNumber(Item->Quantity));

	if (ItemIcon && Item->ItemIcon)
		ItemIcon->SetBrushFromTexture(Item->ItemIcon);
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
	
	ItemQuantity->SetText(FText::AsNumber(InventorySlot.Quantity));
}

void UInventoryItemWidget::InitSlot(const FInventorySlot& InventorySlot)
{
	CachedSlotID = InventorySlot.SlotID;
	SetItemData(InventorySlot.ItemData);
	UpdateQuantity(InventorySlot);
}
