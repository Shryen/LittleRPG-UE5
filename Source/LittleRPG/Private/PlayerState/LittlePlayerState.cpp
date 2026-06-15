

#include "PlayerState/LittlePlayerState.h"

#include "Data/FInventorySlot.h"
#include "Data/ItemData.h"
#include "Net/UnrealNetwork.h"

ALittlePlayerState::ALittlePlayerState()
{
	bReplicates = true;
}

void ALittlePlayerState::AddItemToInventory(UItemData* Item)
{
	if (!Item)
		return;
	
	if (!HasAuthority())
		return;
	
	for (FInventorySlot& Slot : Inventory)
		if (Slot.ItemData == Item && Slot.Quantity < Slot.ItemData->MaxStack)
		{
			Slot.Quantity++;
			Server_OnSlotChanged(Slot);
			return;
		}
	
	FInventorySlot NewSlot;
	NewSlot.SlotID   = NextSlotID++; 
	NewSlot.ItemData = Item;
	NewSlot.Quantity = 1;
	
	Inventory.Add(NewSlot);
	Server_OnSlotChanged(NewSlot);
	PrintInventory();
}

void ALittlePlayerState::PrintInventory()
{
	if (!HasAuthority())
		return;
	
	if (Inventory.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ALittlePlayerState::PrintInventory: Inventory is empty"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("PrintInventory: %s has %d slots:"), *GetPlayerName(), Inventory.Num());
	
	
	for (int32 Index = 0; Index < Inventory.Num(); ++Index)
	{
		const FInventorySlot& InventorySlot = Inventory[Index];
		if (!InventorySlot.ItemData)
		{
			UE_LOG(LogTemp, Warning, TEXT("  [%d] SlotID: %d | Empty slot"), Index, InventorySlot.SlotID);
			continue;
		}
        
		UE_LOG(LogTemp, Warning, TEXT("  [%d] SlotID: %d | Item: %s | Qty: %d"),
			Index,
			InventorySlot.SlotID,
			*InventorySlot.ItemData->ItemName.ToString(),
			InventorySlot.Quantity);
	}
}

void ALittlePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALittlePlayerState, Inventory);
}

void ALittlePlayerState::Server_OnSlotChanged_Implementation(const FInventorySlot& Slot)
{
	OnInventorySlotChanged.Broadcast(Slot);
}

void ALittlePlayerState::OnRep_Inventory()
{
	for (const FInventorySlot& Slot : Inventory)
	{
		OnInventorySlotChanged.Broadcast(Slot);
	}
}
