#include "HUD/Widget/Inventory/InventoryWidget.h"

#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void UInventoryWidget::AddItem(const FText& ItemName)
{
	UE_LOG(LogTemp, Warning, TEXT("Trying to add text to inventory widget"));
	if (!InventoryContainerBox) return;

	UTextBlock* Text = NewObject<UTextBlock>(this);
	Text->SetText(ItemName);
	InventoryContainerBox->AddChild(Text);
	UE_LOG(LogTemp, Warning, TEXT("Widget added successfully"));

}

void UInventoryWidget::ClearItems()
{
	if (InventoryContainerBox)
		InventoryContainerBox->ClearChildren();
}
