#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LittleInventoryManagerComponent.generated.h"

class UItemData;
struct FInventorySlot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChanged, const FInventorySlot&);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULittleInventoryManagerComponent();

	void AddItemToInventory(UItemData* Item);
	

	UFUNCTION(BlueprintCallable)
	void TestClearInventory();

	UFUNCTION(Server, Reliable)
	void Server_EquipItem(UItemData* Item);
	
	void PrintInventory();

	FOnInventorySlotChanged OnInventorySlotChanged;

	TArray<FInventorySlot>& GetInventory() {return Inventory;};

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetNextSlotID() const {return NextSlotID;};
	void SetNextSlotID(const int32 NewNextSlotID) {NextSlotID = NewNextSlotID;};

	UFUNCTION(NetMulticast, Reliable)
	void Server_OnSlotChanged(const FInventorySlot& Slot);

protected:
	virtual void BeginPlay() override;

private:
	int32 NextSlotID = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Inventory)
	TArray<FInventorySlot> Inventory;

	UFUNCTION()
	void OnRep_Inventory();
};
