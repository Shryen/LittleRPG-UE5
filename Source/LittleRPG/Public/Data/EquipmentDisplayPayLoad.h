#pragma once

#include "CoreMinimal.h"
#include "Data/EquipmentSlot.h"
#include "EquipmentDisplayPayload.generated.h"

USTRUCT(BlueprintType)
struct FEquipmentDisplayPayload
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly) EEquipmentSlot SlotType = EEquipmentSlot::Empty;

	UPROPERTY(BlueprintReadOnly) FName ItemRowName;

	UPROPERTY(BlueprintReadOnly) FText ItemName;
	UPROPERTY(BlueprintReadOnly) UTexture2D* ItemIcon = nullptr;
};