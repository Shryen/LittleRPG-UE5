#include "HUD/Widget/Inventory/Equipment/EquipmentPanelWidget.h"

#include "Components/VerticalBox.h"
#include "Data/EquipmentDisplayPayLoad.h"
#include "Data/EquipmentSlot.h"
#include "HUD/Widget/Inventory/Equipment/EquipmentSlotWidget.h"

void UEquipmentPanelWidget::UpdateFromPayload(const FEquipmentDisplayPayload& Payload)
{
	if (UEquipmentSlotWidget* Widget = GetSlotWidget(Payload.SlotType))
		Widget->UpdateFromPayload(Payload);
}

void UEquipmentPanelWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BindSlotWidget(Slot_Head,    EEquipmentSlot::Head);
	BindSlotWidget(Slot_Chest,   EEquipmentSlot::Chest);
	BindSlotWidget(Slot_Legs,    EEquipmentSlot::Legs);
	BindSlotWidget(Slot_Feet,    EEquipmentSlot::Feet);
	BindSlotWidget(Slot_Weapon,  EEquipmentSlot::Weapon);
	BindSlotWidget(Slot_OffHand, EEquipmentSlot::OffHand);
	BindSlotWidget(Slot_Ring,    EEquipmentSlot::Ring);
	BindSlotWidget(Slot_Trinket, EEquipmentSlot::Trinket);
	BindSlotWidget(Slot_Lantern, EEquipmentSlot::Lantern);
}

UEquipmentSlotWidget* UEquipmentPanelWidget::GetSlotWidget(EEquipmentSlot SlotType) const
{
	switch (SlotType)
	{
	case EEquipmentSlot::Head:    return Slot_Head;
	case EEquipmentSlot::Chest:   return Slot_Chest;
	case EEquipmentSlot::Legs:    return Slot_Legs;
	case EEquipmentSlot::Feet:    return Slot_Feet;
	case EEquipmentSlot::Weapon:  return Slot_Weapon;
	case EEquipmentSlot::OffHand: return Slot_OffHand;
	case EEquipmentSlot::Ring:    return Slot_Ring;
	case EEquipmentSlot::Trinket: return Slot_Trinket;
	case EEquipmentSlot::Lantern: return Slot_Lantern;
	default:                      return nullptr;
	}
}

void UEquipmentPanelWidget::BindSlotWidget(UEquipmentSlotWidget* Widget, EEquipmentSlot SlotType)
{
	if (!Widget) return;
	Widget->SlotType = SlotType;
	Widget->OnEquipmentSlotRightClicked.AddUObject(this, &UEquipmentPanelWidget::OnSlotRightClicked);
}

void UEquipmentPanelWidget::OnSlotRightClicked(EEquipmentSlot SlotType)
{
	OnEquipmentSlotRightClicked.Broadcast(SlotType);
}
