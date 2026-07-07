#pragma once

#include "CoreMinimal.h"
#include "Data/Equipment/EquipmentSlot.h"
#include "ItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FText ItemName;
	UPROPERTY(EditAnywhere) FText ItemDescription;
	UPROPERTY(EditAnywhere) TSoftObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere) int32 ItemValue = 0;
	UPROPERTY(EditAnywhere) int32 MaxStack = 1;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere) EEquipmentSlot EquipmentSlot = EEquipmentSlot::Empty;
	UPROPERTY(EditAnywhere) FName AttachmentSocket;
};