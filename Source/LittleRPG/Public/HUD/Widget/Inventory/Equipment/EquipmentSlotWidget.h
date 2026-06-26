#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "EquipmentSlotWidget.generated.h"

enum class EEquipmentSlot : uint8;
class UBorder;
class UImage;
/**
 * 
 */
UCLASS()
class LITTLERPG_API UEquipmentSlotWidget : public ULittleUserWidget
{
	GENERATED_BODY()
public:
	
	void SetItem(FName ItemRowName);
	void ClearSlot();

	UPROPERTY(EditDefaultsOnly, Category="EquipmentSlot|Data")
	TObjectPtr<UDataTable> ItemDataTable;
	
	EEquipmentSlot SlotType;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> BackgroundBorder;
};
