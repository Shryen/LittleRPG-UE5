#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "LittleGameplayAbility.generated.h"

/**
 * 
 */
UCLASS()
class LITTLERPG_API ULittleGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	ULittleGameplayAbility();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TObjectPtr<UTexture2D> AbilityIcon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FText AbilityDisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag InputTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	FGameplayTag CooldownTag;
};
