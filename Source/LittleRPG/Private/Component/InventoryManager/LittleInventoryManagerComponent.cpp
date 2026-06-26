#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/Equipment/EquipmentDisplayPayLoad.h"
#include "Data/Equipment/EquipmentSlot.h"
#include "Data/Equipment/FEquipmentArray.h"
#include "Data/Inventory/FInventoryArray.h"
#include "Data/Inventory/FInventorySlot.h"
#include "Data/Inventory/ItemDataRow.h"
#include "Data/Inventory/InventoryDisplayPayLoad.h"
#include "Net/UnrealNetwork.h"

/* FInventorySlot callbacks */

void FInventorySlot::PostReplicatedAdd(const FInventoryArray& InArraySerializer)
{
	if (ULittleInventoryManagerComponent* Comp = InArraySerializer.OwnerComponent.Get())
		Comp->NotifyInventorySlotChanged(*this);
}

void FInventorySlot::PostReplicatedChange(const FInventoryArray& InArraySerializer)
{
	if (ULittleInventoryManagerComponent* Comp = InArraySerializer.OwnerComponent.Get())
		Comp->NotifyInventorySlotChanged(*this);
}

/* FEquipmentSlot callbacks */

void FEquipmentSlot::PostReplicatedAdd(const FEquipmentArray& InArraySerializer)
{
	if (auto* Comp = InArraySerializer.OwnerComponent.Get())
		Comp->NotifyEquipmentSlotChanged(*this);
}

void FEquipmentSlot::PostReplicatedChange(const FEquipmentArray& InArraySerializer)
{
	if (auto* Comp = InArraySerializer.OwnerComponent.Get())
		Comp->NotifyEquipmentSlotChanged(*this);
}

// Callbacks end

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
	bool bArraySizeChanged = false;

	for (FInventorySlot& Slot : Inventory.Items)
	{
		if (Slot.ItemRowName != ItemRowName)
			continue;
		if (Slot.Quantity >= Row->MaxStack)
			continue;

		const int32 Room  = Row->MaxStack - Slot.Quantity;
		const int32 ToAdd = FMath::Min(Remaining, Room);
		Slot.Quantity += ToAdd;
		Remaining     -= ToAdd;
		
		Inventory.MarkItemDirty(Slot);
		
		if (Remaining <= 0) return;
	}

	while (Remaining > 0)
	{
        FInventorySlot& NewSlot = Inventory.Items.AddDefaulted_GetRef();
		NewSlot.SlotID = NextSlotID++;
		NewSlot.ItemRowName = ItemRowName;
		NewSlot.Quantity = FMath::Min(Remaining, Row->MaxStack);
		NewSlot.VisualSlotIndex = NextVisualIndex++;
		
		// Inventory.Items.Add(NewSlot); AddDefaulted_GetRef adds the item to the inventory, don't add again
		Remaining -= NewSlot.Quantity;
		bArraySizeChanged = true;
	}
	
	if (bArraySizeChanged)
	{
		// When elemetn been added or removed must mark the array
		Inventory.MarkArrayDirty();
	}
	
	PrintInventory();
}

void ULittleInventoryManagerComponent::PrintInventory()
{
	for (const FInventorySlot& Slot : Inventory.Items)
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
	
	FInventorySlot* InvSlot = Inventory.Items.FindByPredicate(
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
	
	const FName ItemToEquip     = InvSlot->ItemRowName;
	const FName PreviouslyEquip = EquipSlot->ItemRowName;
	
	if (!PreviouslyEquip.IsNone())
	{
		InvSlot->ItemRowName = PreviouslyEquip;
		InvSlot->Quantity    = 1;
	}else {
		InvSlot->ItemRowName = NAME_None;
		InvSlot->Quantity    = 0;
	}
	Inventory.MarkItemDirty(*InvSlot);
	
	EquipSlot->ItemRowName = ItemToEquip;
	EquipmentSlots.MarkItemDirty(*EquipSlot);
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
	EquipmentSlots.MarkItemDirty(*EquipSlot);
}

FEquipmentSlot* ULittleInventoryManagerComponent::FindEquipmentSlot(EEquipmentSlot SlotType)
{
	const uint8 Index = (uint8)SlotType;
	if (EquipmentSlots.Items.IsValidIndex(Index))
		return &EquipmentSlots.Items[Index];
	return nullptr;
}

void ULittleInventoryManagerComponent::NotifyInventorySlotChanged(const FInventorySlot& InSlot) const
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
		Payload.ItemIcon = Row->ItemIcon;
	}
	
	OnSlotDisplayDirty.Broadcast(Payload);
}

void ULittleInventoryManagerComponent::NotifyEquipmentSlotChanged(const FEquipmentSlot& InSlot) const
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
		Payload.ItemIcon = Row->ItemIcon;
	}
	
	OnEquipmentSlotDirty.Broadcast(Payload);
}

void ULittleInventoryManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	Inventory.OwnerComponent = this;
	EquipmentSlots.OwnerComponent = this;

	if (GetOwner()->HasAuthority())
	{
		const uint8 SlotCount = static_cast<uint8>(EEquipmentSlot::Count);
		for (uint8 i = 0; i < SlotCount; i++)
		{
			FEquipmentSlot& Slot = EquipmentSlots.Items.AddDefaulted_GetRef();
			Slot = FEquipmentSlot(static_cast<EEquipmentSlot>(i));
		}
		EquipmentSlots.MarkArrayDirty();
	}
}
