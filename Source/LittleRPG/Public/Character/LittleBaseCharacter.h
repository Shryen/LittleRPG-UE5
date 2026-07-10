#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"
#include "LittleBaseCharacter.generated.h"

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
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AbilitySystem")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AbilitySystem")
	EGameplayEffectReplicationMode AscReplicationMode = EGameplayEffectReplicationMode::Mixed;
	
private:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USceneComponent> LightMagicSceneComponent;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULittleEquipManager> EquipManager;
	
	UPROPERTY(EditDefaultsOnly, Category = "AbilitySystem")
	TArray<TSubclassOf<UGameplayAbility>> GivenAbilities;
};
