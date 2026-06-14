#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LittlePlayerState.generated.h"

struct FInventorySlot;
class UItemData;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChanged, const FInventorySlot&);

UCLASS()
class LITTLERPG_API ALittlePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ALittlePlayerState();
	
	FOnInventorySlotChanged OnInventorySlotChanged;
	
	const TArray<FInventorySlot>& GetInventory() const {return Inventory;};
	
	void AddItemToInventory(UItemData* Item);
	
	void PrintInventory();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Inventory)
	TArray<FInventorySlot> Inventory;
	
	UFUNCTION(NetMulticast, Reliable)
	void Server_OnSlotChanged(const FInventorySlot& Slot);
	
	UFUNCTION()
	void OnRep_Inventory();
	
private:
	int32 NextSlotID = 1;
};
