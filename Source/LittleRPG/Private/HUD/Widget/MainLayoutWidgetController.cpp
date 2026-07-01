#include "HUD/Widget/MainLayoutWidgetController.h"
#include "HUD/Widget/MainLayoutWidget.h"
#include "Components/Button.h"
#include "Components/Overlay.h"
#include "Components/WidgetSwitcher.h"

void UMainLayoutWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	
	MainLayoutWidget = Cast<UMainLayoutWidget>(InWidget);
	if (!MainLayoutWidget)
	{
		UE_LOG(LogTemp, Warning, TEXT("UMainLayoutWidgetController::SetWidget: Cast to UMainLayoutWidget failed!"));
		return;
	}
	
	if (MainLayoutWidget->InventoryOverlay)
		MainLayoutWidget->InventoryOverlay->SetVisibility(ESlateVisibility::Collapsed);
	
	if (MainLayoutWidget->InventoryButton)
		MainLayoutWidget->InventoryButton->OnClicked.AddDynamic(this, &UMainLayoutWidgetController::SwitchToInventory);
	
	if (MainLayoutWidget->CraftingButton)
		MainLayoutWidget->CraftingButton->OnClicked.AddDynamic(this, &UMainLayoutWidgetController::SwitchToCrafting);
}

void UMainLayoutWidgetController::ToggleInventory()
{
	bInventoryOpen ? HideInventoryWidget() : ShowInventoryWidget();
}

void UMainLayoutWidgetController::SwitchWidget(EWidgetPage Page)
{
	MainLayoutWidget->WidgetSwitcher->SetActiveWidgetIndex(static_cast<uint8>(Page));
	UE_LOG(LogTemp, Warning, TEXT("Switched to widget: %d"), static_cast<uint8>(Page));
}

void UMainLayoutWidgetController::SwitchToInventory()
{
	SwitchWidget(EWidgetPage::Inventory);
}

void UMainLayoutWidgetController::SwitchToCrafting()
{
	SwitchWidget(EWidgetPage::Crafting);
}

void UMainLayoutWidgetController::ShowInventoryWidget()
{
	if (!MainLayoutWidget || !MainLayoutWidget->InventoryOverlay)
		return;
	
	MainLayoutWidget->InventoryOverlay->SetVisibility(ESlateVisibility::Visible);
	bInventoryOpen = true;
	
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void UMainLayoutWidgetController::HideInventoryWidget()
{
	if (!MainLayoutWidget || !MainLayoutWidget->InventoryOverlay)
		return;
	
	MainLayoutWidget->InventoryOverlay->SetVisibility(ESlateVisibility::Collapsed);
	bInventoryOpen = false;
	
	FInputModeGameOnly InputMode;
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = false;
}
