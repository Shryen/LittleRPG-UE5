#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "EquipmentPanelWidget.generated.h"

class UEquipmentSlotWidget;
class UVerticalBox;
enum class EEquipmentSlot : uint8;

UCLASS()
class LITTLERPG_API UEquipmentPanelWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
    void UpdateSlot(EEquipmentSlot SlotType, FName ItemRowName);
	void ClearAllSlots();
	void SetItemDataTable(UDataTable* InDataTable) { ItemDataTable = InDataTable; }
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> SlotContainer;

	UPROPERTY(EditDefaultsOnly, Category = "Equipment")
	TSubclassOf<UEquipmentSlotWidget> SlotWidgetClass;
	
private:
	UPROPERTY()
	TArray<TObjectPtr<UEquipmentSlotWidget>> SlotWidgets;

	UPROPERTY()
	TObjectPtr<UDataTable> ItemDataTable;
};
