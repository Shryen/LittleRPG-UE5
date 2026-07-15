#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "LittleAbilitySystemComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

protected:
	virtual void OnRep_ActivateAbilities() override;
	
private:
	UPROPERTY()
	TArray<FGameplayAbilitySpec> LastActivatableAbilities;
};
