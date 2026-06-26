#include "HUD/Widget/Inventory/Equipment/EquipmentSlotWidget.h"
#include "Engine/DataTable.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include "Data/ItemDataRow.h"

void UEquipmentSlotWidget::SetItem(FName ItemRowName)
{
	if (ItemRowName.IsNone())
	{
		ClearSlot();
		return;
	}
	
	if (!ItemDataTable)
		return;
	
	const FItemDataRow* Row = nullptr;
	
	Row = ItemDataTable->FindRow<FItemDataRow>(ItemRowName, "");
	if (!Row)
		return;
	
	if (ItemIcon)
		ItemIcon->SetBrushFromTexture(Row->ItemIcon.LoadSynchronous());
}

void UEquipmentSlotWidget::ClearSlot()
{
	ItemIcon->SetBrushFromTexture(nullptr);
	BackgroundBorder->SetBrushColor(FColor::Black);
}
