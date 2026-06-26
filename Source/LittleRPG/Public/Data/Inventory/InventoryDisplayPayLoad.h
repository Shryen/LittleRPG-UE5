#pragma once
 
#include "CoreMinimal.h"
#include "InventoryDisplayPayload.generated.h"

USTRUCT(BlueprintType)
struct FInventoryDisplayPayload
{
	GENERATED_BODY()
 
	UPROPERTY(BlueprintReadOnly) int32 VisualSlotIndex = INDEX_NONE;
 
	UPROPERTY(BlueprintReadOnly) int32 Quantity = 0;
 
	UPROPERTY(BlueprintReadOnly) FText ItemName;
	UPROPERTY(BlueprintReadOnly) TSoftObjectPtr<UTexture2D> ItemIcon = nullptr;
};
 