#include "HUD/Widget/Crafting/CraftingWidget.h"
#include "HUD/Widget/Crafting/CraftingSlotWidget.h"
#include "Components/UniformGridPanel.h"
#include "Data/Crafting/CraftSlot.h"
#include "Data/Crafting/IngredientData.h"
#include "Data/Inventory/ItemDataRow.h"

void UCraftingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	PopulateSlots();
}

void UCraftingWidget::PopulateSlots()
{
	SlotWidgets.Empty();

	if (!CraftingGrid || !CraftingSlotWidgetClass || !CraftingData)
		return;

	TArray<FName> AllRecipes = CraftingData->GetRowNames();
	
	for (int i=0; i<AllRecipes.Num(); ++i)
	{
		FName CurrentRowName = AllRecipes[i];
		
		FIngredientRecipeData* RecipeData = CraftingData->FindRow<FIngredientRecipeData>(CurrentRowName, TEXT("Crafting Grid"));
		
		if (!RecipeData) return;
		
		FName ResultItem = RecipeData->ResultItemRowName;
		FItemDataRow* ItemVisuals = InventoryDataTable->FindRow<FItemDataRow>(ResultItem, TEXT("Crafting Visuals"));
		if (ItemVisuals == nullptr) return;
		
		UCraftingSlotWidget* SlotWidget = CreateWidget<UCraftingSlotWidget>(this, CraftingSlotWidgetClass);
		SlotWidget->InitSlot(CurrentRowName);
		SlotWidget->OnSlotClicked.AddLambda([this](FName ItemRowName)
		{
			OnCraftRequested.Broadcast(ItemRowName);
		});
		
		FCraftSlot CraftSlot;
		CraftSlot.Name = ItemVisuals->ItemName;
		CraftSlot.Icon = ItemVisuals->ItemIcon;
		SlotWidget->UpdateSlot(CraftSlot);
		
		int32 Row = i / ColumnCount;
		int32 Column = i % ColumnCount;
		CraftingGrid->AddChildToUniformGrid(SlotWidget, Row, Column);
	}
}