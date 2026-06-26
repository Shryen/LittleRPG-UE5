#include "HUD/Widget/Inventory/InventoryWidgetController.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/EquipmentDisplayPayLoad.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
#include "HUD/Widget/Inventory/Equipment/EquipmentPanelWidget.h"
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
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	LittlePlayerState = Cast<ALittlePlayerState>(PlayerState);
}

void UInventoryWidgetController::ToggleInventory()
{
	bIsOpen ? HideInventoryWidget() : ShowInventoryWidget();
}

void UInventoryWidgetController::BindDependencies()
{
    LittlePlayerState = PlayerController ? PlayerController->GetPlayerState<ALittlePlayerState>() : nullptr;
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

void UInventoryWidgetController::ShowInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	InventoryWidget->SetVisibility(ESlateVisibility::Visible);
	bIsOpen = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(InventoryWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void UInventoryWidgetController::HideInventoryWidget() 
{
	if (!InventoryWidget)
		return;
	
	InventoryWidget->SetVisibility(ESlateVisibility::Collapsed);
	bIsOpen = false;
	
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}

