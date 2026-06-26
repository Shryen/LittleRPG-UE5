#include "Component/InventoryManager/LittleInventoryManagerComponent.h"

#include "Data/FInventorySlot.h"
#include "Data/ItemDataRow.h"
#include "Net/UnrealNetwork.h"

ULittleInventoryManagerComponent::ULittleInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void ULittleInventoryManagerComponent::AddItemToInventory(const FName& ItemRowName, int32 Quantity)
{
	if (ItemRowName.IsNone() || !ItemDataTable || Quantity <= 0)
		return;
	if (!GetOwner()->HasAuthority())
		return;

	const FItemDataRow* Row = ItemDataTable->FindRow<FItemDataRow>(ItemRowName, "");
	if (!Row) return;

	int32 Remaining = Quantity;

	for (FInventorySlot& Slot : Inventory)
	{
		if (Slot.ItemRowName == ItemRowName && Slot.Quantity < Row->MaxStack)
		{
			int32 Room = Row->MaxStack - Slot.Quantity;
			int32 ToAdd = FMath::Min(Remaining, Room);
			Slot.Quantity += ToAdd;
			Remaining -= ToAdd;
			OnInventorySlotChanged.Broadcast(Slot);
			if (Remaining <= 0) return;
		}
	}

	while (Remaining > 0)
	{
		FInventorySlot NewSlot;
		NewSlot.SlotID = NextSlotID++;
		NewSlot.ItemRowName = ItemRowName;
		NewSlot.Quantity = FMath::Min(Remaining, Row->MaxStack);
		Inventory.Add(NewSlot);
		OnInventorySlotChanged.Broadcast(NewSlot);
		Remaining -= NewSlot.Quantity;
	}
	
	PrintInventory();
}

void ULittleInventoryManagerComponent::PrintInventory()
{
	for (const FInventorySlot& Slot : Inventory)
	{
		UE_LOG(LogTemp, Warning, TEXT("[%d] Item in Inventory: %s"), Slot.SlotID, *Slot.ItemRowName.ToString());
	}
}

void ULittleInventoryManagerComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULittleInventoryManagerComponent, Inventory);
}

void ULittleInventoryManagerComponent::OnRep_Equipment()
{
	
}
