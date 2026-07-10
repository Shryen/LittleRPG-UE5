#include "AbilitySystem/Abilities/LittleGameplayAbility.h"

ULittleGameplayAbility::ULittleGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
}
