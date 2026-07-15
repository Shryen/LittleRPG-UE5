// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayTagContainer.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "AbilityBarWidgetController.generated.h"

class UAbilityBarWidget;
class UAbilitySlot;
/**
 * 
 */
struct FAbilityBarSlot;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FOnAbilityBarChanged,
	const TArray<FAbilityBarSlot>&,
	Slots
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(
	FOnAbilityCooldownChanged,
	FGameplayTag, CooldownTag,
	float, TimeRemaining,
	float, CooldownDuration,
	bool, bIsOnCooldown
);

UCLASS(Blueprintable, BlueprintType)
class LITTLERPG_API UAbilityBarWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
	
public:
	void OnAbilitiesChanged(const FGameplayEventData* GameplayEventData);
	void BindDependencies();
	
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	
	UPROPERTY(BlueprintAssignable)
	FOnAbilityBarChanged OnAbilityBarChanged;

	UPROPERTY(BlueprintAssignable)
	FOnAbilityCooldownChanged OnAbilityCooldownChanged;

private:
	void RefreshAbilities();
	
	UPROPERTY()
	TObjectPtr<UAbilityBarWidget> AbilityBarWidget;
};
