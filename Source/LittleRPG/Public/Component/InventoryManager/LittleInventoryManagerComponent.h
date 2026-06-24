#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LittleInventoryManagerComponent.generated.h"

class AActor;
class UItemData;
struct FInventorySlot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChanged, const FInventorySlot&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquippedItemChanged, UItemData*);

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
	void Server_EquipItem(UItemData* Item, AActor* SpawnedActor);

	UFUNCTION(Server, Reliable)
	void Server_UnequipItem();

	void PrintInventory();

	FOnInventorySlotChanged OnInventorySlotChanged;
	FOnEquippedItemChanged OnEquippedItemChanged;

	TArray<FInventorySlot>& GetInventory() {return Inventory;};

	UItemData* GetEquippedItem() const { return EquippedItem; }

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

	UPROPERTY(ReplicatedUsing = OnRep_EquippedItem)
	TObjectPtr<UItemData> EquippedItem;

	TObjectPtr<AActor> EquippedActor;

	UFUNCTION()
	void OnRep_EquippedItem();
};
