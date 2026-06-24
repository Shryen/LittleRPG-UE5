#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

class APickupObject;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Resource    UMETA(DisplayName = "Resource"),
	Consumable  UMETA(DisplayName = "Consumable"),
	Equipment   UMETA(DisplayName = "Equipment"),
	QuestItem   UMETA(DisplayName = "Quest Item")
};

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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemType ItemType = EItemType::Resource;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag ItemTag;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AActor> ItemClass; 
};
