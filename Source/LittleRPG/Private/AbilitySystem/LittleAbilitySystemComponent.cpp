#include "AbilitySystem/LittleAbilitySystemComponent.h"

#include "Character/LittleBaseCharacter.h"

void ULittleAbilitySystemComponent::OnRep_ActivateAbilities()
{
	Super::OnRep_ActivateAbilities();
	
	ALittleBaseCharacter* BaseCharacter = Cast<ALittleBaseCharacter>(GetOwner());
	if (!BaseCharacter) return;
	
	
	bool bAbilitiesChanged = false;
	
	// So we're not calling RefreshAbilities on UI everytime something happens
	if (LastActivatableAbilities.Num() != ActivatableAbilities.Items.Num())
		bAbilitiesChanged = true;
	else // If the size same but the ability different so we replace the old ability to new
		for (int32 i=0; i<ActivatableAbilities.Items.Num(); i++)
			if (LastActivatableAbilities[i].Ability != ActivatableAbilities.Items[i].Ability)
			{
				bAbilitiesChanged = true;
				break;
			}
	
	
	if (bAbilitiesChanged)
	{
		BaseCharacter->SendAbilitiesChangedEvent();
		LastActivatableAbilities = ActivatableAbilities.Items;
	}
}

