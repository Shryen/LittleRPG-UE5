#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LittlePlayerState.generated.h"

class UItemData;

DECLARE_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS()
class LITTLERPG_API ALittlePlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	ALittlePlayerState();
	
	FOnInventoryChanged OnInventoryChanged;
	
	const TArray<TObjectPtr<UItemData>>& GetInventory() const {return Inventory;};
	
	void AddItem(UItemData* Item);
	
	void PrintInventory();
protected:
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	UPROPERTY(VisibleAnywhere, Replicated)
	TArray<TObjectPtr<UItemData>> Inventory;
};
