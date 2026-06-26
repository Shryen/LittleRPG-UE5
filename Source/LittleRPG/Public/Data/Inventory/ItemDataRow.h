#pragma once

#include "CoreMinimal.h"
#include "Data/Equipment/EquipmentSlot.h"
#include "GameplayTagContainer.h"
#include "ItemDataRow.generated.h"

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FText ItemName;
	UPROPERTY(EditAnywhere) FText ItemDescription;
	UPROPERTY(EditAnywhere) TSoftObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere) TSoftObjectPtr<UStaticMesh> ItemMesh;
	UPROPERTY(EditAnywhere) int32 ItemValue = 0;
	UPROPERTY(EditAnywhere) int32 MaxStack = 1;
	UPROPERTY(EditAnywhere) FGameplayTag ItemTag;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere) EEquipmentSlot EquipmentSlot = EEquipmentSlot::Empty;
};