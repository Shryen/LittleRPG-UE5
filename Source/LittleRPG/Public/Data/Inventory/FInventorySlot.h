#pragma once

#include "CoreMinimal.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "FInventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot : public FFastArraySerializerItem  
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 SlotID = INDEX_NONE;
	UPROPERTY(VisibleAnywhere) int32 VisualSlotIndex = INDEX_NONE;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FName ItemRowName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int32 Quantity = 0;
	
	bool operator==(const FInventorySlot& Other) const
	{
		return SlotID == Other.SlotID
			&& VisualSlotIndex == Other.VisualSlotIndex
			&& ItemRowName == Other.ItemRowName
			&& Quantity == Other.Quantity;
	}

	void PostReplicatedAdd(const struct FInventoryArray& InArraySerializer);
	void PostReplicatedChange(const struct FInventoryArray& InArraySerializer);
};	
