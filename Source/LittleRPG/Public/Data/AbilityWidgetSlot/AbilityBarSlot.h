#pragma once
#include "GameplayAbilitySpecHandle.h"
#include "GameplayTagContainer.h"
#include "AbilityBarSlot.generated.h"

USTRUCT(BlueprintType)
struct FAbilityBarSlot
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UTexture2D> Icon = nullptr;
	
	UPROPERTY(BlueprintReadOnly)
	FText DisplayName;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag InputTag;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayTag CooldownTag;
	
	UPROPERTY(BlueprintReadOnly)
	FGameplayAbilitySpecHandle SpecHandle; 
	
	UPROPERTY(BlueprintReadOnly)
	bool bShouldShowInBar = false; 
};
