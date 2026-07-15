#pragma once

#include "EquipmentConfig.generated.h"

class UGameplayAbility;
class UGameplayEffect;
class ULittleGameplayAbility;

USTRUCT(Blueprintable)
struct FEquipmentConfig
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly) TSubclassOf<UAnimInstance> AnimInstance;
	UPROPERTY(EditDefaultsOnly) TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant;
	UPROPERTY(EditDefaultsOnly) TArray<TSubclassOf<UGameplayEffect>> EffectsToGrant;
};