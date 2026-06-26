#pragma once

#include "Net/Serialization/FastArraySerializer.h"
#include "EquipmentSlot.generated.h"

UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Head, Chest, Legs, Weapon, OffHand,
	Ring, Feet, Trinket, Lantern,
	Empty, Count UMETA(Hidden)

};

USTRUCT(BlueprintType)
struct FEquipmentSlot : public FFastArraySerializerItem
{
	GENERATED_BODY()
	FEquipmentSlot() : SlotType(EEquipmentSlot::Empty), InventorySlotID(INDEX_NONE){}
	FEquipmentSlot(const EEquipmentSlot InType) : SlotType(InType) {}
	
	UPROPERTY()
	EEquipmentSlot SlotType = EEquipmentSlot::Empty;

	UPROPERTY()
	int32 InventorySlotID = INDEX_NONE;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemRowName = NAME_None;
	
	bool operator==(const FEquipmentSlot& Other) const
	{
		return SlotType == Other.SlotType
			&& ItemRowName == Other.ItemRowName;
	}

	void PostReplicatedAdd(const struct FEquipmentArray& InArraySerializer);
	void PostReplicatedChange(const struct FEquipmentArray& InArraySerializer);
};