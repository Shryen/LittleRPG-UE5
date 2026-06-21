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
		
	TArray<FInventorySlot>& GetInventory() {return Inventory;};
	
	void AddItemToInventory(UItemData* Item);
	
	UFUNCTION(BlueprintCallable)
	void TestClearInventory();
	
	void PrintInventory();
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UFUNCTION(NetMulticast, Reliable)
	void Server_OnSlotChanged(const FInventorySlot& Slot);
	
	UFUNCTION()
	void OnRep_Inventory();
	
	int32 GetNextSlotID() const {return NextSlotID;};
	void SetNextSlotID(const int32 NewNextSlotID) {NextSlotID = NewNextSlotID;};
	
private:
	int32 NextSlotID = 1;
	
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Inventory)
	TArray<FInventorySlot> Inventory;
};
