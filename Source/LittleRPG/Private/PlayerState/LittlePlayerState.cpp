

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
	if (Inventory.IsEmpty())
	{
		return;
	}
	
	if (!HasAuthority())
		return;
	
	for (int32 Index = 0; Index < Inventory.Num(); ++Index)
	{
		const FInventorySlot& Slot = Inventory[Index];
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
