#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LittleCraftManagerComponent.generated.h"


class ULittleInventoryManagerComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleCraftManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULittleCraftManagerComponent();
	
	virtual void BeginPlay() override;
	
	bool HasIngredients(FName RecipeRowName);
	void ExecuteCrafting(FName RecipeRowName);
	void AddCraftedItemToInventory(FName ItemRowName);
	
	UFUNCTION(Server, Reliable)
	void Server_ExecuteCrafting(FName RecipeRowName);
	
	int32 GetInventoryItemCount(FName ItemRowName);
	void RemoveItemFromInventory(FName ItemRowName, int32 Amount);
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Crafting")
	UDataTable* CraftingDataTable;
	
	UPROPERTY()
	TObjectPtr<ULittleInventoryManagerComponent> InventoryManager;
};
