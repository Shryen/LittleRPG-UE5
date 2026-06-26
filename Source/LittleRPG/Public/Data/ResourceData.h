#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "ResourceData.generated.h"

/**
 * 
 */
UCLASS()
class LITTLERPG_API UResourceData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	FGameplayTag ResourceTag;
	
	UPROPERTY(EditDefaultsOnly)
	UTexture2D* ResourceIcon;
        
	UPROPERTY(EditDefaultsOnly)
	FName DropsItemRowName;
};
