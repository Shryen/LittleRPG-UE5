#include "HUD/Widget/Inventory/InventoryWidgetController.h"

#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/FInventorySlot.h"
#include "HUD/Widget/Inventory/InventoryWidget.h"
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
	BindDependencies();
}

void UInventoryWidgetController::ToggleInventory()
{
	bIsOpen ? HideInventoryWidget() : ShowInventoryWidget();
}

void UInventoryWidgetController::BindDependencies()
{
	LittlePlayerState->GetInventoryManager()->OnInventorySlotChanged.AddUObject(this, &UInventoryWidgetController::OnInventoryChanged);
}

void UInventoryWidgetController::OnInventoryChanged(const FInventorySlot& SlotChanged)
{
	InventoryWidget->UpdateSlot(SlotChanged);
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

