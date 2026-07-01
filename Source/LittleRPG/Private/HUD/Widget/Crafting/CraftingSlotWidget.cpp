#include "HUD/Widget/Crafting/CraftingSlotWidget.h"
#include "Data/Crafting/CraftSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCraftingSlotWidget::UpdateSlot(const FCraftSlot& InSlot)
{
	if (SlotName)
		SlotName->SetText(InSlot.Name);

	if (SlotIcon)
		SlotIcon->SetBrushFromSoftTexture(InSlot.Icon);
}
