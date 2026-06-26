#include "HUD/Widget/Inventory/Equipment/EquipmentSlotWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Data/Equipment/EquipmentDisplayPayLoad.h"
#include "Engine/DataTable.h"

void UEquipmentSlotWidget::UpdateFromPayload(const FEquipmentDisplayPayload& Payload)
{
	if (Payload.ItemRowName.IsNone())
	{
		bIsOccupied = false;
		if (ItemIcon)         ItemIcon->SetBrushFromTexture(nullptr);
		if (BackgroundBorder) BackgroundBorder->SetBrushColor(FLinearColor(0.05f, 0.05f, 0.05f, 1.f));
		return;
	}
	
	bIsOccupied = true;
	if (ItemIcon) ItemIcon->SetBrushFromSoftTexture(Payload.ItemIcon);
	if (BackgroundBorder) BackgroundBorder->SetBrushColor(FLinearColor(0.15f, 0.12f, 0.08f, 1.f)); // slight gold tint when occupied
}

void UEquipmentSlotWidget::NativeConstruct()
{
	Super::NativeConstruct();
	bIsOccupied = false;
	if (ItemIcon)         ItemIcon->SetBrushFromTexture(nullptr);
	if (BackgroundBorder) BackgroundBorder->SetBrushColor(FLinearColor(0.05f, 0.05f, 0.05f, 1.f));
}

FReply UEquipmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
 
	if (InMouseEvent.GetEffectingButton() == EKeys::RightMouseButton && bIsOccupied)
	{
		OnEquipmentSlotRightClicked.Broadcast(SlotType);
		UE_LOG(LogTemp, Warning, TEXT("UEquipmentSlotWidget: right-clicked slot [%d]"), (uint8)SlotType);
		return FReply::Handled();
	}
	return FReply::Unhandled();
}
