#pragma once

#include "CoreMinimal.h"
#include "Data/Equipment/EquipmentConfig.h"
#include "Data/Equipment/EquipmentSlot.h"
#include "ItemDataRow.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FItemDataRow : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere) FText ItemName;
	UPROPERTY(EditAnywhere) FText ItemDescription;
	UPROPERTY(EditAnywhere) int32 ItemLevel = 1;
	UPROPERTY(EditAnywhere) TObjectPtr<UTexture2D> ItemIcon;
	UPROPERTY(EditAnywhere) int32 ItemValue = 0;
	UPROPERTY(EditAnywhere) int32 MaxStack = 1;
	UPROPERTY(EditAnywhere) TSubclassOf<AActor> ItemClass;
	UPROPERTY(EditAnywhere) EEquipmentSlot EquipmentSlot = EEquipmentSlot::Empty;
	UPROPERTY(EditAnywhere) FName AttachmentSocket;
	UPROPERTY(EditAnywhere) FEquipmentConfig EquipmentConfig;
	UPROPERTY(EditAnywhere) int32 EquipmentEffectLevel = 1;
};