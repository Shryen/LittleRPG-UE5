#pragma once

#include "CoreMinimal.h"
#include "CraftSlot.generated.h"

USTRUCT(BlueprintType)
struct FCraftSlot
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> Icon;
};
