#include "Component/InventoryManager/LittleInventoryManagerComponent.h"

#include "Data/EquipmentDisplayPayLoad.h"
#include "Data/FInventorySlot.h"
#include "Data/ItemDataRow.h"
#include "Data/InventoryDisplayPayLoad.h"
#include "Net/UnrealNetwork.h"

ULittleInventoryManagerComponent::ULittleInventoryManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
	
	const uint8 SlotCount = static_cast<uint8>(EEquipmentSlot::Count);
	EquipmentSlots.SetNum(SlotCount);
	for (uint8 i = 0; i < SlotCount; i++)
		EquipmentSlots[i] = FEquipmentSlot(static_cast<EEquipmentSlot>(i));
	
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
		if (Slot.ItemRowName != ItemRowName)
			continue;
		if (Slot.Quantity >= Row->MaxStack)
			continue;

		const int32 Room  = Row->MaxStack - Slot.Quantity;
		const int32 ToAdd = FMath::Min(Remaining, Room);
		Slot.Quantity += ToAdd;
		Remaining     -= ToAdd;
		
		BroadcastInventorySlotPayload(Slot);
		
		if (Remaining <= 0) return;
	}

	while (Remaining > 0)
	{
		FInventorySlot NewSlot;
		NewSlot.SlotID = NextSlotID++;
		NewSlot.ItemRowName = ItemRowName;
		NewSlot.Quantity = FMath::Min(Remaining, Row->MaxStack);
		NewSlot.VisualSlotIndex = NextVisualIndex++;
		
		Inventory.Add(NewSlot);
		Remaining -= NewSlot.Quantity;
		
		BroadcastInventorySlotPayload(NewSlot);
	}
	
	PrintInventory();
}

void ULittleInventoryManagerComponent::PrintInventory()
{
	for (const FInventorySlot& Slot : Inventory)
		UE_LOG(LogTemp, Warning, TEXT("[Visual %d | ID %d] %s x%d"),
					Slot.VisualSlotIndex, Slot.SlotID, *Slot.ItemRowName.ToString(), Slot.Quantity);
}

void ULittleInventoryManagerComponent::GetLifetimeReplicatedProps(
	TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ULittleInventoryManagerComponent, Inventory);
	DOREPLIFETIME(ULittleInventoryManagerComponent, EquipmentSlots);
}

void ULittleInventoryManagerComponent::Server_EquipItemFromInventory_Implementation(int32 VisualSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("[Server_EquipItemFromInventory] VisualSlotIndex=%d HasAuthority=%d"), VisualSlotIndex, GetOwner() ? GetOwner()->HasAuthority() : false);
	EquipItemFromInventory(VisualSlotIndex);
}

void ULittleInventoryManagerComponent::Server_UnequipItem_Implementation(EEquipmentSlot SlotType)
{
	UnequipItem(SlotType);
}

void ULittleInventoryManagerComponent::EquipItemFromInventory(int32 VisualSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("[EquipItemFromInventory] VisualSlotIndex=%d"), VisualSlotIndex);
	if (!GetOwner()->HasAuthority()) { UE_LOG(LogTemp, Error, TEXT("[EquipItemFromInventory] NO AUTHORITY")); return; }
	if (!ItemDataTable) { UE_LOG(LogTemp, Error, TEXT("[EquipItemFromInventory] ItemDataTable is NULL")); return; }
	
	FInventorySlot* InvSlot = Inventory.FindByPredicate(
		[VisualSlotIndex](const FInventorySlot& S) { return S.VisualSlotIndex == VisualSlotIndex; });
	
	if (!InvSlot || InvSlot->ItemRowName.IsNone() || InvSlot->Quantity <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("[EquipItemFromInventory] InvSlot not found or empty"));
		return;
	}
	
	UE_LOG(LogTemp, Warning, TEXT("[EquipItemFromInventory] Found slot: %s x%d"), *InvSlot->ItemRowName.ToString(), InvSlot->Quantity);
	
	const FItemDataRow* Row = ItemDataTable->FindRow<FItemDataRow>(InvSlot->ItemRowName, "");
	if (!Row) { UE_LOG(LogTemp, Error, TEXT("[EquipItemFromInventory] Row not found in DataTable")); return; }
	
	if (Row->EquipmentSlot == EEquipmentSlot::Empty) { UE_LOG(LogTemp, Error, TEXT("[EquipItemFromInventory] EquipmentSlot is Empty")); return; }
	
	FEquipmentSlot* EquipSlot = FindEquipmentSlot(Row->EquipmentSlot);
	if (!EquipSlot) return;
	
	// Get what we want to equip
	const FName ItemToEquip     = InvSlot->ItemRowName;
	const FName PreviouslyEquip = EquipSlot->ItemRowName;
	
	if (!PreviouslyEquip.IsNone())
	{
		InvSlot->ItemRowName = PreviouslyEquip;
		InvSlot->Quantity    = 1; // equipment is always 1, cannot stack
	}else {
		InvSlot->ItemRowName = NAME_None;
		InvSlot->Quantity    = 0; // This tells the widget to hide
	}
	BroadcastInventorySlotPayload(*InvSlot);
	
	EquipSlot->ItemRowName = ItemToEquip;
	BroadcastEquipmentSlotPayload(*EquipSlot);
}

void ULittleInventoryManagerComponent::UnequipItem(EEquipmentSlot SlotType)
{
	if (!GetOwner()->HasAuthority())
		return;
	
	FEquipmentSlot* EquipSlot = FindEquipmentSlot(SlotType);
	if (!EquipSlot || EquipSlot->ItemRowName.IsNone())
		return;
 
	AddItemToInventory(EquipSlot->ItemRowName, 1);
	EquipSlot->ItemRowName = NAME_None;
	BroadcastEquipmentSlotPayload(*EquipSlot);
}


void ULittleInventoryManagerComponent::OnRep_Equipment()
{
	for (const FEquipmentSlot& New : EquipmentSlots)
	{
		const FEquipmentSlot* Old = LastKnownEquipment.FindByPredicate(
			[&](const FEquipmentSlot& S) { return S.SlotType == New.SlotType; });
 
		if (!Old || Old->ItemRowName != New.ItemRowName)
			BroadcastEquipmentSlotPayload(New);
	}
 
	LastKnownEquipment = EquipmentSlots;
}

void ULittleInventoryManagerComponent::OnRep_Inventory()
{
	for (const FInventorySlot& NewSlot : Inventory)
	{ 
		// FindByPredicate for custom logic, this case: SlotID
		const FInventorySlot* OldSlot = LastKnownInventory.FindByPredicate(
			[&](const FInventorySlot& S) { return S.SlotID == NewSlot.SlotID; });
		
		const bool bIsNew = (OldSlot == nullptr); // Slot was empty before
		const bool bQtyChanged = OldSlot && (OldSlot->Quantity != NewSlot.Quantity); // Quantity doesn't match
		const bool bRowChanged = OldSlot && (OldSlot->ItemRowName != NewSlot.ItemRowName); // Name doesn't match
		if (bIsNew || bQtyChanged || bRowChanged)
			BroadcastInventorySlotPayload(NewSlot);
	}
	
	for (const FInventorySlot& OldSlot : LastKnownInventory)
	{
		const bool bStillExists = Inventory.ContainsByPredicate(
			[&](const FInventorySlot& S) { return S.SlotID == OldSlot.SlotID; });
 
		if (!bStillExists)
		{
			FInventoryDisplayPayload ClearPayload;
			ClearPayload.VisualSlotIndex = OldSlot.VisualSlotIndex;
			ClearPayload.Quantity        = 0;
			OnSlotDisplayDirty.Broadcast(ClearPayload);
		}
	}
	
	LastKnownInventory = Inventory;
}

FEquipmentSlot* ULittleInventoryManagerComponent::FindEquipmentSlot(EEquipmentSlot SlotType)
{
	const uint8 Index = (uint8)SlotType;
	if (EquipmentSlots.IsValidIndex(Index))
		return &EquipmentSlots[Index];
	return nullptr;
}

void ULittleInventoryManagerComponent::BroadcastInventorySlotPayload(const FInventorySlot& InSlot) const
{
	FInventoryDisplayPayload Payload;
	Payload.VisualSlotIndex = InSlot.VisualSlotIndex;
	Payload.Quantity        = InSlot.Quantity;
	
	if (!ItemDataTable) return;
	if (InSlot.ItemRowName.IsNone()) 
	{
		OnSlotDisplayDirty.Broadcast(Payload);
		return;
	}
	
	if (const FItemDataRow* Row = ItemDataTable->FindRow<FItemDataRow>(InSlot.ItemRowName, ""))
	{
		Payload.ItemName = Row->ItemName;
		Payload.ItemIcon = Row->ItemIcon.IsNull() ? nullptr : Row->ItemIcon.LoadSynchronous();	
	}
	
	OnSlotDisplayDirty.Broadcast(Payload);
}

void ULittleInventoryManagerComponent::BroadcastEquipmentSlotPayload(const FEquipmentSlot& InSlot) const
{
	FEquipmentDisplayPayload Payload;
	Payload.SlotType = InSlot.SlotType;
	Payload.ItemRowName = InSlot.ItemRowName;
	
	if (!ItemDataTable) return;
	if (InSlot.ItemRowName.IsNone())
	{
	    OnEquipmentSlotDirty.Broadcast(Payload);
		return;
	}
	
	if (const FItemDataRow* Row = ItemDataTable->FindRow<FItemDataRow>(InSlot.ItemRowName, ""))
	{
		Payload.ItemName = Row->ItemName;
		Payload.ItemIcon = Row->ItemIcon.IsNull() ? nullptr : Row->ItemIcon.LoadSynchronous();
	}
	
	OnEquipmentSlotDirty.Broadcast(Payload);

}

