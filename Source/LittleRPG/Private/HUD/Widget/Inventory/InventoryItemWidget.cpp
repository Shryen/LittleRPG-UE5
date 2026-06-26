#include "HUD/Widget/Inventory/InventoryItemWidget.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Engine/DataTable.h"
#include "Data/InventoryDisplayPayLoad.h"

void UInventoryItemWidget::UpdateFromPayload(const FInventoryDisplayPayload& Payload)
{
	if (Payload.Quantity <= 0)
	{
		SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	
	SetVisibility(ESlateVisibility::Visible);
	
	if (!ItemIcon) return;
	if (!ItemQuantity) return;
	
	if (Payload.ItemIcon)
		ItemIcon->SetBrushFromTexture(Payload.ItemIcon.LoadSynchronous());
	else
		ItemIcon->SetBrushFromTexture(nullptr);
	
	if (Payload.Quantity > 1)
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Visible);
		ItemQuantity->SetText(FText::AsNumber(Payload.Quantity));
	}
	else
	{
		ItemQuantity->SetVisibility(ESlateVisibility::Hidden);
	}
}

FReply UInventoryItemWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
 
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		OnItemClicked.Broadcast(VisualSlotIndex);
        UE_LOG(LogTemp, Warning, TEXT("Item was clicked [%d]"), VisualSlotIndex);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
