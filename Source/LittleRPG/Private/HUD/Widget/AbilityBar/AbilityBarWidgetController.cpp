#include "HUD/Widget/AbilityBar/AbilityBarWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/LittleGameplayAbility.h"
#include "AbilitySystem/GameplayTags/LittleGameplayTags.h"
#include "Data/AbilityWidgetSlot/AbilityBarSlot.h"
#include "HUD/Widget/AbilityBar/AbilityBarWidget.h"


void UAbilityBarWidgetController::OnAbilitiesChanged(const FGameplayEventData* GameplayEventData)
{
	RefreshAbilities();
}

void UAbilityBarWidgetController::BindDependencies()
{
	if (!AbilitySystemComponent) return;
	
	FGameplayEventMulticastDelegate& AbilitiesChangedDelegate =
		AbilitySystemComponent->GenericGameplayEventCallbacks.FindOrAdd(TAG_Event_Abilities_Changed);
	AbilitiesChangedDelegate.RemoveAll(this);
	AbilitiesChangedDelegate.AddUObject(this, &ThisClass::OnAbilitiesChanged);

	RefreshAbilities();
}

void UAbilityBarWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	Super::SetWidget(InWidget);
	AbilityBarWidget = Cast<UAbilityBarWidget>(InWidget);
}


void UAbilityBarWidgetController::RefreshAbilities()
{
	if (!AbilitySystemComponent) return;
	TArray<FAbilityBarSlot> Slots;
	for (const FGameplayAbilitySpec& Spec : AbilitySystemComponent->GetActivatableAbilities())
	{
		const ULittleGameplayAbility* Ability =  Cast<ULittleGameplayAbility>(Spec.Ability);
		if (!Ability) continue;
		
		FAbilityBarSlot Slot;
		Slot.Icon = Ability->AbilityIcon;
		Slot.DisplayName = Ability->AbilityDisplayName;
		Slot.InputTag = Ability->InputTag;
		Slot.CooldownTag = Ability->CooldownTag;
		Slot.SpecHandle = Spec.Handle;
		Slot.bShouldShowInBar = Ability->bShouldShowInAbilityBar;
		
		Slots.Add(Slot);
	}
	
	OnAbilityBarChanged.Broadcast(Slots);
}

