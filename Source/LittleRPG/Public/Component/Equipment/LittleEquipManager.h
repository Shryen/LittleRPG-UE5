#pragma once

#include "CoreMinimal.h"
#include "Actor/Equipment/Equipment.h"
#include "Components/ActorComponent.h"
#include "GameplayEffectTypes.h"
#include "Data/Equipment/EquipmentDisplayPayLoad.h"
#include "LittleEquipManager.generated.h"

class UGameplayEffect;
class ULittleInventoryManagerComponent;
class ALittlePlayerState;
class UAbilitySystemComponent;
struct FItemDataRow;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleEquipManager : public UActorComponent
{
	GENERATED_BODY()

public:
	ULittleEquipManager();
	
	void UpdateAnimInstanceClass(TSubclassOf<UAnimInstance> NewClass);
	void OnEquipmentChanged(const FEquipmentDisplayPayload& EquipmentDisplayPayload);
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void BeginPlay() override;

protected:
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
private:
	// PlayerState
	UPROPERTY()
	TObjectPtr<ALittlePlayerState> LittlePlayerState;
	UFUNCTION()
	void OnPlayerStateReady();
	bool SetupPlayerState();
	FItemDataRow* GetRowFromDataTable(FName ItemRowName) const;
	AEquipment* DefferEquipment(const FItemDataRow* Row) const;
	void AttachToMesh(EEquipmentSlot Slot, const FItemDataRow* Row, AEquipment* Equipment) const;

	// Equipment
	UPROPERTY()
	TMap<EEquipmentSlot, TObjectPtr<AActor>> SpawnedEquipment;
	
	TMap<EEquipmentSlot, TArray<FActiveGameplayEffectHandle>> ActiveGameplayEffects;
	
	void DestroyEquippedActor(EEquipmentSlot Slot);
	AActor* SpawnEquippedActor(EEquipmentSlot Slot, FName ItemRowName);
	FName GetDefaultSocket(EEquipmentSlot Slot) const;

	UPROPERTY()
	TObjectPtr<ULittleInventoryManagerComponent> InventoryManager;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|Animation")
	TSubclassOf<UAnimInstance> UnarmedAnimClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|Animation", ReplicatedUsing = OnRep_AnimInstance)
	TSubclassOf<UAnimInstance> CurrentAnimInstanceClass;
	
	UFUNCTION()
	void OnRep_AnimInstance(TSubclassOf<UAnimInstance> OldAnimInstance) const;
};
