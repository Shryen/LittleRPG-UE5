UENUM(BlueprintType)
enum class EEquipmentSlot : uint8
{
	Head, Chest, Legs, Weapon, OffHand,
	Ring, Feet, Trinket, Lantern,
	Count  // for array sizing — not a real slot
};