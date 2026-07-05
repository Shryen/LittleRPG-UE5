#pragma once

#include "IngredientData.generated.h"

USTRUCT(BlueprintType)
struct FIngredientData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly) FName ItemRowName;	// example wood or stone
	UPROPERTY(EditDefaultsOnly) int32 Amount;		// how much of wood or stone we need to craft
};


USTRUCT(BlueprintType)
struct FIngredientRecipeData : public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly) TArray<FIngredientData> Ingredients; // array of ingredients, 3 stone + 2 wood = axe for example
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ResultItemRowName;
};
