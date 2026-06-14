#include "HUD/Widget/Inventory/InventoryWidgetController.h"
#include "Data/FInventorySlot.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "PlayerState/LittlePlayerState.h"

void UInventoryWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	InventoryWidget = Cast<UInventoryWidget>(InWidget);
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidgetController::SetWidget: Cast to UInventoryWidget failed!"));
		return;
	}
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryWidgetController::ToggleInventory()
{
	bIsOpen ? HideInventoryWidget() : ShowInventoryWidget();
}

void UInventoryWidgetController::OnSlotChanged(const FInventorySlot& InventorySlot)
{
	if (!InventoryWidget) return;
	InventoryWidget->UpdateSlot(InventorySlot);
}

void UInventoryWidgetController::ShowInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	bIsOpen = true;
}

void UInventoryWidgetController::HideInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	bIsOpen = false;
}

void UInventoryWidgetController::BindPlayerStateToInventory(ALittlePlayerState* PS)
{
	PlayerState = PS;
	if(!PS)
		return;
	
	PS->OnInventorySlotChanged.AddUObject(this, &UInventoryWidgetController::OnSlotChanged);
	RefreshInventory();
}

void UInventoryWidgetController::RefreshInventory()
{
	ALittlePlayerState* PS = Cast<ALittlePlayerState>(PlayerState);
	if (!PS) return;

	for (const FInventorySlot& Slot : PS->GetInventory())
	{
		OnSlotChanged(Slot);
	}
}
