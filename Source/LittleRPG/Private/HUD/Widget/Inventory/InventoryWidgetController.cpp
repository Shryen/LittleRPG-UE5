#include "HUD/Widget/Inventory/InventoryWidgetController.h"

#include "Data/ItemData.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "PlayerState/LittlePlayerState.h"

void UInventoryWidgetController::SetWidget(UInventoryWidget* InWidget)
{
	InventoryWidget = InWidget;
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UInventoryWidgetController::ToggleInventory()
{
	bIsOpen ? HideInventoryWidget() : ShowInventoryWidget();
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

void UInventoryWidgetController::PopulateInventory() const
{
	ALittlePlayerState* PS = Cast<ALittlePlayerState>(PlayerState);
	if (!PS || !InventoryWidget) return;

	InventoryWidget->ClearItems();
	
	for (const UItemData* Item : PS->GetInventory())
	{
		if (!Item)
		{
			UE_LOG(LogTemp, Warning, TEXT("ALittleHUD::PopulateInventory — null item in inventory, skipping"));
			continue; 
		}
		InventoryWidget->AddItem(Item->ItemName);
	}
}

void UInventoryWidgetController::BindPlayerStateToInventory(APlayerState* InPlayerState)
{
	ALittlePlayerState* PS = Cast<ALittlePlayerState>(InPlayerState);
	if(!PS)
		return;
	
	PS->OnInventoryChanged.AddUObject(this, &UInventoryWidgetController::PopulateInventory);
	PopulateInventory();
}
