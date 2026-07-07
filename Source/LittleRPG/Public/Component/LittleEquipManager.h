#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Equipment/EquipmentDisplayPayLoad.h"
#include "LittleEquipManager.generated.h"


class ULittleInventoryManagerComponent;
class ALittlePlayerState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleEquipManager : public UActorComponent
{
	GENERATED_BODY()

public:
	ULittleEquipManager();
	
	UFUNCTION()
	void OnEquipmentChanged(const FEquipmentDisplayPayload& EquipmentDisplayPayload);
	
protected:
	virtual void BeginPlay() override;
	
private:
	// PlayerState
	UPROPERTY()
	TObjectPtr<ALittlePlayerState> LittlePlayerState;
	UFUNCTION()
	void OnPlayerStateReady();
	bool SetupPlayerState();
	
	// Equipment
	UPROPERTY()
	TMap<EEquipmentSlot, TObjectPtr<AActor>> SpawnedEquipment;
	
	void DestroyEquippedActor(EEquipmentSlot Slot);
	AActor* SpawnEquippedActor(EEquipmentSlot Slot, FName ItemRowName);
	FName GetDefaultSocket(EEquipmentSlot Slot) const;

	UPROPERTY()
	TObjectPtr<ULittleInventoryManagerComponent> InventoryManager;
	
};
