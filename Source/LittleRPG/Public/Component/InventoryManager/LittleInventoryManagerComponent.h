#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LittleInventoryManagerComponent.generated.h"

struct FEquipmentSlot;
class UDataTable;
struct FInventorySlot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventorySlotChanged, const FInventorySlot&);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULittleInventoryManagerComponent();

	void AddItemToInventory(const FName& ItemRowName, int32 Quantity);
	void PrintInventory();

	FOnInventorySlotChanged OnInventorySlotChanged;

	TArray<FInventorySlot>& GetInventory() {return Inventory;};

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetNextSlotID() const {return NextSlotID;};
	void SetNextSlotID(const int32 NewNextSlotID) {NextSlotID = NewNextSlotID;};

	UDataTable* GetItemDataTable() const { return ItemDataTable; }
	

private:
	int32 NextSlotID = 1;

	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<FInventorySlot> Inventory;
	
	UPROPERTY(ReplicatedUsing=OnRep_Equipment)
	TArray<FEquipmentSlot> EquipmentSlots;
	
	UFUNCTION()
	void OnRep_Equipment();
	
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> ItemDataTable;
};
