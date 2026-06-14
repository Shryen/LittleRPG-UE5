#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "LittleSaveGame.generated.h"

USTRUCT()
struct FSavedInventorySlot
{
	GENERATED_BODY()
	
	UPROPERTY()
	int32 SlotID = INDEX_NONE;
	
	UPROPERTY()
	FSoftObjectPath ItemDataPath; 
	
	UPROPERTY()
	int32 Quantity = 0;
};

/**
 * 
 */
UCLASS()
class LITTLERPG_API ULittleSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	TArray<FSavedInventorySlot> SavedInventory;
	
	UPROPERTY()
	int32 SavedNextSlotID = 1;
};
