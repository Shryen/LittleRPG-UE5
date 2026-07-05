#include "Component/LittleCraftManagerComponent.h"
#include "Component/InventoryManager/LittleInventoryManagerComponent.h"
#include "Data/Crafting/IngredientData.h"
#include "PlayerState/LittlePlayerState.h"


class ULittleInventoryManagerComponent;

ULittleCraftManagerComponent::ULittleCraftManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULittleCraftManagerComponent::BeginPlay()
{
	Super::BeginPlay();
	if (AActor* Owner = GetOwner())
	{
		if (ALittlePlayerState* PS = Cast<ALittlePlayerState>(Owner))
			InventoryManager = PS->GetInventoryManager();
	}
}

bool ULittleCraftManagerComponent::HasIngredients(FName RecipeRowName)
{
	if (!CraftingDataTable) return false;
	FIngredientRecipeData* RecipeData = CraftingDataTable->FindRow<FIngredientRecipeData>(RecipeRowName, TEXT("CraftManager Check"));
	if (!RecipeData) return false;
	
	for (const FIngredientData& Ingredient : RecipeData->Ingredients)
	{
		if (GetInventoryItemCount(Ingredient.ItemRowName) < Ingredient.Amount)
		{
			return false; 
		}
	}
	
	return true;
}

void ULittleCraftManagerComponent::ExecuteCrafting(FName RecipeRowName)
{
	if (!CraftingDataTable) return;
	FIngredientRecipeData* RecipeData = CraftingDataTable->FindRow<FIngredientRecipeData>(RecipeRowName, TEXT("CraftManager Check"));
	if (!HasIngredients(RecipeRowName)) return;
	for (const FIngredientData& Ingredient : RecipeData->Ingredients)
	{
		RemoveItemFromInventory(Ingredient.ItemRowName, Ingredient.Amount);
	}
	AddCraftedItemToInventory(RecipeData->ResultItemRowName);
}

void ULittleCraftManagerComponent::AddCraftedItemToInventory(FName ItemRowName)
{
	if (!InventoryManager) return;
	InventoryManager->AddItemToInventory(ItemRowName, 1);
}

int32 ULittleCraftManagerComponent::GetInventoryItemCount(FName ItemRowName)
{
	if (!InventoryManager) return 0;
	int32 Count = 0;
	
	for (const FInventorySlot& InSlot : InventoryManager->GetInventory().Items)
	{
		if (InSlot.ItemRowName == ItemRowName)
			Count += InSlot.Quantity;
	}
	return Count;
}

void ULittleCraftManagerComponent::RemoveItemFromInventory(FName ItemRowName, int32 Amount)
{
	if (!InventoryManager) return;
	for (FInventorySlot& Slot : InventoryManager->GetInventory().Items)
	{
		if (Slot.ItemRowName != ItemRowName || Slot.Quantity <= 0)
			continue;
		
		int32 ToRemove = FMath::Min(Amount, Slot.Quantity);
		Slot.Quantity -= ToRemove;
		Amount -= ToRemove;
		
		if (Slot.Quantity == 0)
			Slot.ItemRowName = NAME_None;
		
		InventoryManager->NotifyInventorySlotChanged(Slot);
		InventoryManager->GetInventory().MarkItemDirty(Slot);
		
		if (Amount <= 0)
			break;
	}
}

void ULittleCraftManagerComponent::Server_ExecuteCrafting_Implementation(FName RecipeRowName)
{
	ExecuteCrafting(RecipeRowName);
}


