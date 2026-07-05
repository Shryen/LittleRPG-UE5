#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/Inventory/FInventoryArray.h"
#include "Data/Equipment/FEquipmentArray.h"
#include "LittleInventoryManagerComponent.generated.h"

struct FEquipmentDisplayPayload;
enum class EEquipmentSlot : uint8;
struct FInventoryDisplayPayload;
struct FEquipmentSlot;
class UDataTable;
struct FInventorySlot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnSlotDisplayDirty, const FInventoryDisplayPayload&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotDirty, const FEquipmentDisplayPayload&);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class LITTLERPG_API ULittleInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	ULittleInventoryManagerComponent();
	
	// Inventory
	void AddItemToInventory(const FName& ItemRowName, int32 Quantity);
	void PrintInventory();
	void NotifyInventorySlotChanged(const FInventorySlot& Slot) const;

	FOnSlotDisplayDirty OnSlotDisplayDirty;

	const FInventoryArray& GetInventory() const {return Inventory;};
	FInventoryArray& GetInventory() {return Inventory;};

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	int32 GetNextSlotID() const {return NextSlotID;};
	void SetNextSlotID(const int32 NewNextSlotID) {NextSlotID = NewNextSlotID;};

	UDataTable* GetItemDataTable() const { return ItemDataTable; }
	
	static const int32 MaxVisualSlots = 30;
	
	// Equipment
	FOnEquipmentSlotDirty OnEquipmentSlotDirty;
	void EquipItemFromInventory(int32 VisualSlotIndex);
	void UnequipItem(EEquipmentSlot SlotType);
	void NotifyEquipmentSlotChanged(const FEquipmentSlot& Slot) const;
	
	UFUNCTION(Server, Reliable)
	void Server_EquipItemFromInventory(int32 VisualSlotIndex);
	
	UFUNCTION(Server, Reliable)
	void Server_UnequipItem(EEquipmentSlot SlotType);

	const FEquipmentArray& GetEquipmentSlots() const { return EquipmentSlots; }
	
protected:
	virtual void BeginPlay() override;
	
private:
	int32 NextSlotID  = 1;
	int32 NextVisualIndex = 0;

	// Inventory
	UPROPERTY(VisibleAnywhere, Replicated)
	FInventoryArray Inventory;
	
	// Equipment
	UPROPERTY(VisibleAnywhere, Replicated)
	FEquipmentArray EquipmentSlots;
 
	FEquipmentSlot* FindEquipmentSlot(EEquipmentSlot SlotType);
	
	// Data Table
	UPROPERTY(EditDefaultsOnly, Category = "Data")
	TObjectPtr<UDataTable> ItemDataTable;
};
