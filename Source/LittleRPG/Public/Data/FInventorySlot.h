#pragma once

#include "CoreMinimal.h"
#include "FInventorySlot.generated.h"

class UItemData;

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere)
	int32 SlotID = INDEX_NONE;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UItemData> ItemData;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 Quantity = 0;
};
