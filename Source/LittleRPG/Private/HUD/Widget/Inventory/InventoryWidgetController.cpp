#include "HUD/Widget/Inventory/InventoryWidgetController.h"

#include "Data/ItemData.h"
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
	if (!PS || !InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidgetController::PopulateInventory: Cast to UInventoryWidget failed!"));
		return;
	};

	InventoryWidget->ClearItems();
	
	for (const UItemData* Item : PS->GetInventory())
	{
		if (!Item)
		{
			UE_LOG(LogTemp, Warning, TEXT("UInventoryWidgetController::PopulateInventory — null item in inventory, skipping"));
			continue; 
		}
		InventoryWidget->AddItem(Item);
	}
}

void UInventoryWidgetController::BindPlayerStateToInventory(ALittlePlayerState* PS)
{
	PlayerState = PS;
	if(!PS)
		return;
	
	PS->OnInventoryChanged.AddUObject(this, &UInventoryWidgetController::PopulateInventory);
	PopulateInventory();
}
