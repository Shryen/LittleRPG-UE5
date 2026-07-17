#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "LittleBaseCharacter.generated.h"

class ULittleAbilitySystemComponent;
class ULittleAttributeSet;
enum class EGameplayEffectReplicationMode;
class ULittleEquipManager;

DECLARE_MULTICAST_DELEGATE(FOnPlayerStateReadySignature)

UCLASS()
class LITTLERPG_API ALittleBaseCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALittleBaseCharacter();
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintCallable, Category = "Little|Abilities")
	const FGameplayTag& GetAutoAttackTag() const 
	{ 
		UE_LOG(LogTemp, Warning, TEXT("AttackTag got from BaseCharacter %s"), *AutoAttackTag.ToString())
		return AutoAttackTag; 
	}
	
	void SetAutoAttackTag(const FGameplayTag& NewTag)
	{
		UE_LOG(LogTemp, Warning, TEXT("AttackTag been set for BaseCharacter: %s"), *NewTag.ToString())
		AutoAttackTag = NewTag;
	}
	
	UFUNCTION(BlueprintCallable, Category="Little")
	FVector GetLastDirection() const { return GetLastMovementInputVector(); }
	
	FOnPlayerStateReadySignature OnPlayerStateReady;
	
	UFUNCTION(BlueprintCallable, Category = "Little|AbilitySystem", Server, Reliable)
	void Server_SendGameplayEventToSelf(FGameplayEventData EventData);
	
	USceneComponent* GetLightMagicSceneComponent() const {return LightMagicSceneComponent;}
	
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void BindDependencies();
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	
	// ASC
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UFUNCTION(BlueprintCallable, Category = "Little|AbilitySystem|Abilities")
	TArray<FGameplayAbilitySpecHandle> GrantAbilities(TArray<TSubclassOf<UGameplayAbility>> AbilitiesToGrant);
	
	UFUNCTION(BlueprintCallable, Category = "Little|AbilitySystem|Abilities")
	void RemoveAbilities(TArray<FGameplayAbilitySpecHandle> AbilitiesToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Little|AbilitySystem|Abilities")
	void SendAbilitiesChangedEvent();
	
	void InitDefaultGameplayEffects();
	
	ULittleAttributeSet* GetLittleAttributeSet() { return LittleAttributeSet; };
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|AbilitySystem|Effects")
	TSubclassOf<UGameplayEffect> GE_StaminaRegen;
	UPROPERTY(EditDefaultsOnly, Category = "Little|AbilitySystem|Effects")
	TSubclassOf<UGameplayEffect> GE_HealthRegen;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Little|AbilitySystem")
	TObjectPtr<ULittleAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Little|AbilitySystem")
	TObjectPtr<ULittleAttributeSet> LittleAttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Little|AbilitySystem")
	EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Mixed;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> LightMagicSceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULittleEquipManager> EquipManager;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|AbilitySystem|Starting")
	TArray<TSubclassOf<UGameplayAbility>> StartingAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|AbilitySystem|Starting")
	TSubclassOf<UGameplayEffect> InitialGameplayEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|AbilitySystem|Starting")
	TSubclassOf<UGameplayEffect> InitialSecondaryGameplayEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|AbilitySystem")
	FGameplayTag AutoAttackTag;
};
