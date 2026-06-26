#pragma once
#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "FInventorySlot.h"
#include "FInventoryArray.generated.h"

class ULittleInventoryManagerComponent;

USTRUCT(BlueprintType)
struct FInventoryArray : public FFastArraySerializer
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FInventorySlot> Items;

	TWeakObjectPtr<ULittleInventoryManagerComponent> OwnerComponent;

	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParms)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<
			FInventorySlot, FInventoryArray>(Items, DeltaParms, *this);
	}
};

// Required for NetDeltaSerializer
template <>
struct TStructOpsTypeTraits<FInventoryArray> : public TStructOpsTypeTraitsBase2<FInventoryArray>
{
	enum { WithNetDeltaSerializer = true };
};