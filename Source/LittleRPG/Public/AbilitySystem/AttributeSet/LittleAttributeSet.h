#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "LittleAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)
/**
 * 
 */
UCLASS()
class LITTLERPG_API ULittleAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	ULittleAttributeSet();
	
	// Vital
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, Health);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, Stamina);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, MaxStamina);
	
	// Main
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, Vigor);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, Strength);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, Intellect);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, Agility);
	
	// Secondary
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, CriticalChance);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, Armor);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, AttackPower);
	ATTRIBUTE_ACCESSORS(ULittleAttributeSet, AbilityPower);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Vital
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, Health, OldValue);
	}
	
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, MaxHealth, OldValue);
	}
	
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, Stamina, OldValue);
	}

	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, MaxStamina, OldValue);
	}
	
	// Main
	
	UFUNCTION()
	void OnRep_Strength(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, Strength, OldValue);
	}

	UFUNCTION()
	void OnRep_Intellect(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, Intellect, OldValue);
	}

	UFUNCTION()
	void OnRep_Agility(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, Agility, OldValue);
	}
	
	UFUNCTION()
	void OnRep_Vigor(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, Vigor, OldValue);
	}

	UFUNCTION()
	void OnRep_CriticalChance(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, CriticalChance, OldValue);
	}
	
	UFUNCTION()
	void OnRep_Armor(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, Armor, OldValue);
	}
	
	UFUNCTION()
	void OnRep_AttackPower(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, AttackPower, OldValue);
	}
	
	UFUNCTION()
	void OnRep_AbilityPower(const FGameplayAttributeData& OldValue) const
	{
		GAMEPLAYATTRIBUTE_REPNOTIFY(ULittleAttributeSet, AbilityPower, OldValue);
	}
	

private:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "Little|Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Health;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "Little|Attributes|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "Little|Attributes|Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "Little|Attributes|Stamina", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;
	
	// Main stats
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Vigor, Category = "Little|Attributes|Vigor", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Vigor;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Strength, Category = "Little|Attributes|Strength", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Strength;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Intellect, Category = "Little|Attributes|Intellect", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Intellect;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Agility, Category = "Little|Attributes|Agility", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Agility;
	
	// Secondary
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CriticalChance, Category = "Little|Attributes|CriticalChance", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CriticalChance;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Armor, Category = "Little|Attributes|Armor", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Armor;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AttackPower, Category = "Little|Attributes|AttackPower", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AttackPower;
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_AbilityPower, Category = "Little|Attributes|AbilityPower", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData AbilityPower;

	
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;
};
