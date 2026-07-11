#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "InventoryWidgetController.generated.h"

struct FInventoryDisplayPayload;
struct FEquipmentDisplayPayload;
struct FOnAttributeChangeData;
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
	
	void BindDependencies();
	void BindStatAttributes();
private:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	UPROPERTY()
	TObjectPtr<ALittlePlayerState> LittlePlayerState;
	
	void OnSlotDisplayDirty(const FInventoryDisplayPayload& Payload);
	void OnEquipmentSlotDirty(const FEquipmentDisplayPayload& Payload);
	void OnInventoryItemRightClicked(int32 VisualSlotIndex);
	void OnEquipmentSlotRightClicked(EEquipmentSlot SlotType);

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	void OnMaxStaminaChanged(const FOnAttributeChangeData& Data);
	void OnVigorChanged(const FOnAttributeChangeData& Data);
	void OnStrengthChanged(const FOnAttributeChangeData& Data);
	void OnIntellectChanged(const FOnAttributeChangeData& Data);
	void OnAgilityChanged(const FOnAttributeChangeData& Data);
	void OnCriticalChanceChanged(const FOnAttributeChangeData& Data);
	void OnArmorChanged(const FOnAttributeChangeData& Data);
	void OnAttackPowerChanged(const FOnAttributeChangeData& Data);
	void OnAbilityPowerChanged(const FOnAttributeChangeData& Data);
};
