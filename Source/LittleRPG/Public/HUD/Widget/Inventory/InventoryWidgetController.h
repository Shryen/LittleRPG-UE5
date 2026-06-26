#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "InventoryWidgetController.generated.h"

struct FInventoryDisplayPayload;
struct FEquipmentDisplayPayload;
enum class EEquipmentSlot : uint8;
class UInventoryItemWidget;
struct FInventorySlot;
class ALittlePlayerState;
class UInventoryWidget;

UCLASS()
class LITTLERPG_API UInventoryWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

private:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	bool bIsOpen = false;
	
	UPROPERTY()
	TObjectPtr<ALittlePlayerState> LittlePlayerState;
	
	void OnSlotDisplayDirty(const FInventoryDisplayPayload& Payload);
	void OnEquipmentSlotDirty(const FEquipmentDisplayPayload& Payload);
	void OnInventoryItemRightClicked(int32 VisualSlotIndex);
	void OnEquipmentSlotRightClicked(EEquipmentSlot SlotType);
	
	void ShowInventoryWidget();
	void HideInventoryWidget();
	
	void BindDependencies();
};
