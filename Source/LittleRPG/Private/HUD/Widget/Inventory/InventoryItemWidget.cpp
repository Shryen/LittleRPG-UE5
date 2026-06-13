#include "HUD/Widget/Inventory/InventoryItemWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Data/ItemData.h"

void UInventoryItemWidget::SetItemData(const UItemData* Item)
{
	if (!Item) return;

	if (ItemName)
		ItemName->SetText(Item->ItemName);

	if (ItemIcon && Item->ItemIcon)
		ItemIcon->SetBrushFromTexture(Item->ItemIcon);
}