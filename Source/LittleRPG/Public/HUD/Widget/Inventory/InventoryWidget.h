#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryWidget.generated.h"

struct FInventoryDisplayPayload;
struct FInventorySlot;
class UEquipmentPanelWidget;
class UInventoryItemWidget;
class UUniformGridPanel;
class UHorizontalBox;
class UScrollBox;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemRightClicked, int32 /* VisualSlotIndex */);

UCLASS()
class LITTLERPG_API UInventoryWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	
	void UpdateFromPayload(const FInventoryDisplayPayload& Payload);
	FOnInventoryItemRightClicked OnInventoryItemRightClicked;
	UEquipmentPanelWidget* GetEquipmentPanel() const { return EquipmentPanelWidget; }

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> InventoryContainerBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentPanelWidget> EquipmentPanelWidget;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Inventory")
	TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Inventory")
	int32 ColumnCount = 6;

private:
	UPROPERTY()
	TMap<int32, UInventoryItemWidget*> SlotWidgets;
	
	void OnCellRightClicked(int32 VisualSlotIndex);
};
