#include "Component/InventoryManager/LittleInventoryManagerComponent.h"

#include "Data/FInventorySlot.h"
#include "Data/ItemData.h"
#include "GameFramework/PlayerState.h"
#include "Net/UnrealNetwork.h"

ULittleInventoryManagerComponent::ULittleInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void ULittleInventoryManagerComponent::AddItemToInventory(UItemData* Item)
{
	if (!Item)
		return;

	if (!GetOwner()->HasAuthority())
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


void ULittleInventoryManagerComponent::TestClearInventory()
{
	if (!GetOwner()->HasAuthority()) return;

	Inventory.Empty();

	FInventorySlot ClearSlot;
	ClearSlot.SlotID = INDEX_NONE;
	Server_OnSlotChanged(ClearSlot);

	PrintInventory();
}

void ULittleInventoryManagerComponent::PrintInventory()
{
	if (!GetOwner()->HasAuthority())
		return;

	APlayerState* PS = Cast<APlayerState>(GetOwner());
	if (!PS) return;

	if (Inventory.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ULittleInventoryManagerComponent::PrintInventory: Inventory is empty"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("PrintInventory: %s has %d slots:"), *PS->GetPlayerName(), Inventory.Num());

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

void ULittleInventoryManagerComponent::Server_OnSlotChanged_Implementation(const FInventorySlot& Slot)
{
	OnInventorySlotChanged.Broadcast(Slot);
}

void ULittleInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();
}

void ULittleInventoryManagerComponent::OnRep_Inventory()
{
	for (const FInventorySlot& Slot : Inventory)
	{
		OnInventorySlotChanged.Broadcast(Slot);
	}
}

void ULittleInventoryManagerComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULittleInventoryManagerComponent, Inventory);
}
