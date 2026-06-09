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
	FOnInventoryChanged OnInventoryChanged;
	
	const TArray<TObjectPtr<UItemData>>& GetInventory() const {return Inventory;};
	
	void AddItem(UItemData* Item);
	
	void PrintInventory();
protected:
	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<UItemData>> Inventory;  
};
