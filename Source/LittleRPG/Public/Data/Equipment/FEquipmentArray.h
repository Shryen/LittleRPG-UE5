#pragma once
#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "EquipmentSlot.h"
#include "FEquipmentArray.generated.h"

class ULittleInventoryManagerComponent;

USTRUCT(BlueprintType)
struct FEquipmentArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FEquipmentSlot> Items;

	TWeakObjectPtr<ULittleInventoryManagerComponent> OwnerComponent;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<
			FEquipmentSlot, FEquipmentArray>(Items, DeltaParms, *this);
	}
}; 

template <>
struct TStructOpsTypeTraits<FEquipmentArray> : public TStructOpsTypeTraitsBase2<FEquipmentArray>
{
	enum { WithNetDeltaSerializer = true };
};
