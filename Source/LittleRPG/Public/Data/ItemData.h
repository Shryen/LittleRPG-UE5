#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS()
class LITTLERPG_API UItemData : public UDataAsset
{
	GENERATED_BODY()
public:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FText ItemDescription;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon;   

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UStaticMesh* ItemMesh;     

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ItemValue;
};
