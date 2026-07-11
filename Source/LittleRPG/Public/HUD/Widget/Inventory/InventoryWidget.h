#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryWidget.generated.h"

class UStatLineWidget;
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

	UStatLineWidget* GetHealthStatLine() const { return HealthStatLine; }
	UStatLineWidget* GetMaxHealthStatLine() const { return MaxHealthStatLine; }
	UStatLineWidget* GetStaminaStatLine() const { return StaminaStatLine; }
	UStatLineWidget* GetMaxStaminaStatLine() const { return MaxStaminaStatLine; }
	UStatLineWidget* GetVigorStatLine() const { return VigorStatLine; }
	UStatLineWidget* GetStrengthStatLine() const { return StrengthStatLine; }
	UStatLineWidget* GetIntellectStatLine() const { return IntellectStatLine; }
	UStatLineWidget* GetAgilityStatLine() const { return AgilityStatLine; }
	UStatLineWidget* GetCriticalChanceStatLine() const { return CriticalChanceStatLine; }
	UStatLineWidget* GetArmorStatLine() const { return ArmorStatLine; }
	UStatLineWidget* GetAttackPowerStatLine() const { return AttackPowerStatLine; }
	UStatLineWidget* GetAbilityPowerStatLine() const { return AbilityPowerStatLine; }

protected:
	
	virtual void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> InventoryContainerBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UEquipmentPanelWidget> EquipmentPanelWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> HealthStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> MaxHealthStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> StaminaStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> MaxStaminaStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> VigorStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> StrengthStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> IntellectStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> AgilityStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> CriticalChanceStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> ArmorStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> AttackPowerStatLine;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UStatLineWidget> AbilityPowerStatLine;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Inventory")
	TSubclassOf<UInventoryItemWidget> InventoryItemWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "LittleRPG|Inventory")
	int32 ColumnCount = 6;

private:
	UPROPERTY()
	TMap<int32, UInventoryItemWidget*> SlotWidgets;
	
	void OnCellRightClicked(int32 VisualSlotIndex);
};
