#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "EquipmentPanelWidget.generated.h"

struct FEquipmentDisplayPayload;
class UEquipmentSlotWidget;
class UVerticalBox;
enum class EEquipmentSlot : uint8;


DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentPanelSlotRightClicked, EEquipmentSlot);

UCLASS()
class LITTLERPG_API UEquipmentPanelWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	
	void UpdateFromPayload(const FEquipmentDisplayPayload& Payload);
	
	FOnEquipmentPanelSlotRightClicked OnEquipmentSlotRightClicked;


protected:
	virtual void NativeConstruct() override;
	
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Head;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Chest;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Legs;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Feet;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Weapon;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_OffHand;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Ring;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Trinket;
	UPROPERTY(meta = (BindWidget)) TObjectPtr<UEquipmentSlotWidget> Slot_Lantern;

private:
	UEquipmentSlotWidget* GetSlotWidget(EEquipmentSlot SlotType) const;
	void BindSlotWidget(UEquipmentSlotWidget* Widget, EEquipmentSlot SlotType);
	void OnSlotRightClicked(EEquipmentSlot SlotType);
};
