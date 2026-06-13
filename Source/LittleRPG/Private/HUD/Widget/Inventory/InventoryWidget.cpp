#include "HUD/Widget/Inventory/InventoryWidget.h"

#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"

void UInventoryWidget::AddItem(const FText& ItemName)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to add text to inventory widget"));
	if (!InventoryGridPanel) return;

	UTextBlock* Text = NewObject<UTextBlock>(this);
	Text->SetText(ItemName);
	InventoryGridPanel->AddChild(Text);
	UE_LOG(LogTemp, Warning, TEXT("Widget added successfully"));
}

void UInventoryWidget::ClearItems()
{
	if (InventoryGridPanel)
		InventoryGridPanel->ClearChildren();
}
