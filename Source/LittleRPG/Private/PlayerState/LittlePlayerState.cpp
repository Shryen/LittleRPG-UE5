

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
			OnInventorySlotChanged.Broadcast(Slot);
			OnInventoryChanged.Broadcast();
			return;
		}
	
	FInventorySlot NewSlot;
	NewSlot.SlotID   = NextSlotID++; 
	NewSlot.ItemData = Item;
	NewSlot.Quantity = 1;
	
	Inventory.Add(NewSlot);
	OnInventorySlotChanged.Broadcast(NewSlot);
	OnInventoryChanged.Broadcast();
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
		UE_LOG(LogTemp, Warning,
				TEXT("[%d] %s | Qty: %d | MaxStack: %d"),
				Index,
				*Slot.ItemData->ItemName.ToString(),
				Slot.Quantity,
				Slot.ItemData->MaxStack);
	}
}

void ALittlePlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ALittlePlayerState, Inventory);
}

void ALittlePlayerState::OnRep_Inventory()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_Inventory fired — items: %d"), Inventory.Num());
	OnInventoryChanged.Broadcast();
}
