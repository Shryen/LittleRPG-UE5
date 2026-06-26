#pragma once
#include "EquipmentSlot.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Head, Chest, Legs, Weapon, OffHand,
	Ring, Feet, Trinket, Lantern,
	Empty 
};


USTRUCT(BlueprintType)
struct FEquipmentSlot
{
	GENERATED_BODY()
	FEquipmentSlot() : SlotType(EEquipmentSlot::Empty), InventorySlotID(INDEX_NONE){}

	UPROPERTY()
	EEquipmentSlot SlotType = EEquipmentSlot::Empty;

	UPROPERTY()
	int32 InventorySlotID = INDEX_NONE;
};