#include "HUD/Widget/Crafting/CraftingSlotWidget.h"
#include "Data/Crafting/CraftSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UCraftingSlotWidget::InitSlot(const FName& InItemRowName)
{
	ItemRowName = InItemRowName;
}

void UCraftingSlotWidget::UpdateSlot(const FCraftSlot& InSlot)
{
	if (SlotName)
		SlotName->SetText(InSlot.Name);

	if (SlotIcon)
		SlotIcon->SetBrushFromSoftTexture(InSlot.Icon);
}

FReply UCraftingSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		if (!ItemRowName.IsNone())
			OnSlotClicked.Broadcast(ItemRowName);
		
		return FReply::Handled();
	}
	
	return FReply::Unhandled();
}
