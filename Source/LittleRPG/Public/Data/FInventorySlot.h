#pragma once

#include "CoreMinimal.h"
#include "FInventorySlot.generated.h"

USTRUCT(BlueprintType)
struct FInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere) int32 SlotID = INDEX_NONE;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) FName ItemRowName;
	UPROPERTY(BlueprintReadWrite, EditAnywhere) int32 Quantity = 0;
};	
