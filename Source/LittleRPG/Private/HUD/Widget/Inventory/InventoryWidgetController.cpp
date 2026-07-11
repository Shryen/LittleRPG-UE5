#include "HUD/Widget/Inventory/InventoryWidgetController.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystem/AttributeSet/LittleAttributeSet.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/Equipment/EquipmentDisplayPayLoad.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "HUD/Widget/Inventory/Equipment/EquipmentPanelWidget.h"
#include "HUD/Widget/Stats/StatLineWidget.h"
#include "PlayerState/LittlePlayerState.h"

void UInventoryWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	
	InventoryWidget = Cast<UInventoryWidget>(InWidget);
	if (!InventoryWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UInventoryWidgetController::SetWidget: Cast to UInventoryWidget failed!"));
		return;
	}
	LittlePlayerState = Cast<ALittlePlayerState>(PlayerState);
}

void UInventoryWidgetController::BindDependencies()
{
	APawn* Pawn = PlayerController ? PlayerController->GetPawn() : nullptr;
	LittlePlayerState = Pawn ? Cast<ALittlePlayerState>(Pawn->GetPlayerState()) : nullptr;
	if (!LittlePlayerState) { UE_LOG(LogTemp, Error, TEXT("[BindDependencies] LittlePlayerState is NULL")); return; }
	
	ULittleInventoryManagerComponent* Manager = LittlePlayerState->GetInventoryManager();
	if (!Manager) { UE_LOG(LogTemp, Error, TEXT("[BindDependencies] Manager is NULL")); return; }
	
	UE_LOG(LogTemp, Warning, TEXT("[BindDependencies] Binding OK"));
	
	Manager->OnSlotDisplayDirty.AddUObject(this, &UInventoryWidgetController::OnSlotDisplayDirty);
	Manager->OnEquipmentSlotDirty.AddUObject(this, &UInventoryWidgetController::OnEquipmentSlotDirty);
	
	InventoryWidget->OnInventoryItemRightClicked.AddUObject(this, &UInventoryWidgetController::OnInventoryItemRightClicked);
	InventoryWidget->GetEquipmentPanel()->OnEquipmentSlotRightClicked.AddUObject(this, &UInventoryWidgetController::OnEquipmentSlotRightClicked);
}

void UInventoryWidgetController::OnSlotDisplayDirty(const FInventoryDisplayPayload& Payload)
{
	if (InventoryWidget)
		InventoryWidget->UpdateFromPayload(Payload);
}

void UInventoryWidgetController::OnEquipmentSlotDirty(const FEquipmentDisplayPayload& Payload)
{
	if (InventoryWidget && InventoryWidget->GetEquipmentPanel())
		InventoryWidget->GetEquipmentPanel()->UpdateFromPayload(Payload);
}

void UInventoryWidgetController::OnInventoryItemRightClicked(int32 VisualSlotIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("[InventoryWidgetController] OnInventoryItemRightClicked VisualSlotIndex=%d"), VisualSlotIndex);
	if (!LittlePlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryWidgetController] LittlePlayerState is NULL"));
		return;
	}
	ULittleInventoryManagerComponent* Manager = LittlePlayerState->GetInventoryManager();
	if (!Manager)
	{
		UE_LOG(LogTemp, Error, TEXT("[InventoryWidgetController] Manager is NULL"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("[InventoryWidgetController] Calling Server_EquipItemFromInventory"));
	Manager->Server_EquipItemFromInventory(VisualSlotIndex);
}

void UInventoryWidgetController::OnEquipmentSlotRightClicked(EEquipmentSlot SlotType)
{
	if (!LittlePlayerState) return;
	ULittleInventoryManagerComponent* Manager = LittlePlayerState->GetInventoryManager();
	if (!Manager) return;
	Manager->Server_UnequipItem(SlotType);
}

void UInventoryWidgetController::BindStatAttributes()
{
	if (!AbilitySystemComponent || !InventoryWidget) return;

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetHealthAttribute())
		.AddUObject(this, &ThisClass::OnHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetMaxHealthAttribute())
		.AddUObject(this, &ThisClass::OnMaxHealthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetStaminaAttribute())
		.AddUObject(this, &ThisClass::OnStaminaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetMaxStaminaAttribute())
		.AddUObject(this, &ThisClass::OnMaxStaminaChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetVigorAttribute())
		.AddUObject(this, &ThisClass::OnVigorChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetStrengthAttribute())
		.AddUObject(this, &ThisClass::OnStrengthChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetIntellectAttribute())
		.AddUObject(this, &ThisClass::OnIntellectChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetAgilityAttribute())
		.AddUObject(this, &ThisClass::OnAgilityChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetCriticalChanceAttribute())
		.AddUObject(this, &ThisClass::OnCriticalChanceChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetArmorAttribute())
		.AddUObject(this, &ThisClass::OnArmorChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetAttackPowerAttribute())
		.AddUObject(this, &ThisClass::OnAttackPowerChanged);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		ULittleAttributeSet::GetAbilityPowerAttribute())
		.AddUObject(this, &ThisClass::OnAbilityPowerChanged);

	if (UStatLineWidget* Line = InventoryWidget->GetHealthStatLine())
		Line->SetValues(FName("Health"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetHealthAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetMaxHealthStatLine())
		Line->SetValues(FName("Max Health"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetMaxHealthAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetStaminaStatLine())
		Line->SetValues(FName("Stamina"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetStaminaAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetMaxStaminaStatLine())
		Line->SetValues(FName("Max Stamina"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetMaxStaminaAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetVigorStatLine())
		Line->SetValues(FName("Vigor"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetVigorAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetStrengthStatLine())
		Line->SetValues(FName("Strength"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetStrengthAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetIntellectStatLine())
		Line->SetValues(FName("Intellect"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetIntellectAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetAgilityStatLine())
		Line->SetValues(FName("Agility"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetAgilityAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetCriticalChanceStatLine())
		Line->SetValues(FName("Critical Chance"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetCriticalChanceAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetArmorStatLine())
		Line->SetValues(FName("Armor"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetArmorAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetAttackPowerStatLine())
		Line->SetValues(FName("Attack Power"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetAttackPowerAttribute()));

	if (UStatLineWidget* Line = InventoryWidget->GetAbilityPowerStatLine())
		Line->SetValues(FName("Ability Power"), AbilitySystemComponent->GetNumericAttribute(ULittleAttributeSet::GetAbilityPowerAttribute()));
}

void UInventoryWidgetController::OnHealthChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetHealthStatLine())
		Line->SetValues(FName("Health"), Data.NewValue);
}

void UInventoryWidgetController::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetMaxHealthStatLine())
		Line->SetValues(FName("Max Health"), Data.NewValue);
}

void UInventoryWidgetController::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetStaminaStatLine())
		Line->SetValues(FName("Stamina"), Data.NewValue);
}

void UInventoryWidgetController::OnMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetMaxStaminaStatLine())
		Line->SetValues(FName("Max Stamina"), Data.NewValue);
}

void UInventoryWidgetController::OnVigorChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetVigorStatLine())
		Line->SetValues(FName("Vigor"), Data.NewValue);
}

void UInventoryWidgetController::OnStrengthChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetStrengthStatLine())
		Line->SetValues(FName("Strength"), Data.NewValue);
}

void UInventoryWidgetController::OnIntellectChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetIntellectStatLine())
		Line->SetValues(FName("Intellect"), Data.NewValue);
}

void UInventoryWidgetController::OnAgilityChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetAgilityStatLine())
		Line->SetValues(FName("Agility"), Data.NewValue);
}

void UInventoryWidgetController::OnCriticalChanceChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetCriticalChanceStatLine())
		Line->SetValues(FName("Critical Chance"), Data.NewValue);
}

void UInventoryWidgetController::OnArmorChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetArmorStatLine())
		Line->SetValues(FName("Armor"), Data.NewValue);
}

void UInventoryWidgetController::OnAttackPowerChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetAttackPowerStatLine())
		Line->SetValues(FName("Attack Power"), Data.NewValue);
}

void UInventoryWidgetController::OnAbilityPowerChanged(const FOnAttributeChangeData& Data)
{
	if (UStatLineWidget* Line = InventoryWidget->GetAbilityPowerStatLine())
		Line->SetValues(FName("Ability Power"), Data.NewValue);
}
