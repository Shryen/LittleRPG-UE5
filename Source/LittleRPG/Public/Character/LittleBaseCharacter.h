#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "Data/CharacterState/ECharacterState.h"
#include "GameFramework/Character.h"
#include "LittleBaseCharacter.generated.h"

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
	
	FOnPlayerStateReadySignature OnPlayerStateReady;
	
	USceneComponent* GetLightMagicSceneComponent() const {return LightMagicSceneComponent;}
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	
	UFUNCTION()
	void OnRep_CharacterState();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	void OnHealthChanged(const FOnAttributeChangeData& Data);
	void BindDependencies();
	void OnStaminaChanged(const FOnAttributeChangeData& Data);
	
	void InitDefaultGameplayEffects();
	
	ULittleAttributeSet* GetLittleAttributeSet() { return LittleAttributeSet; };
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_StaminaRegen;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> GE_HealthRegen;
	
	void SetCharacterState(const ECharacterState NewState);
	ECharacterState GetCharacterState() const { return CharacterState; };
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AbilitySystem")
	TObjectPtr<ULittleAttributeSet> LittleAttributeSet;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Mixed;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> LightMagicSceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULittleEquipManager> EquipManager;
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> GivenAbilities;
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> InitialGameplayEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TSubclassOf<UGameplayEffect> InitialSecondaryGameplayEffects;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|State", ReplicatedUsing="OnRep_CharacterState")
	ECharacterState CharacterState = ECharacterState::Unarmed;
};
