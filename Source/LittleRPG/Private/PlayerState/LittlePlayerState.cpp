

#include "PlayerState/LittlePlayerState.h"

#include "Data/ItemData.h"

void ALittlePlayerState::AddItem(UItemData* Item)
{
	if (!Item)
	{
		return;
	}
	
	Inventory.Add(Item);
	OnInventoryChanged.Broadcast();
	PrintInventory();
}

void ALittlePlayerState::PrintInventory()
{
	if (Inventory.IsEmpty())
	{
		return;
	}
	
	for (const UItemData* Item : Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("Item Name: %s"), *Item->ItemName.ToString());
	}
}
