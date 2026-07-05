#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LittlePlayerState.generated.h"

class ULittleCraftManagerComponent;
class ULittleInventoryManagerComponent;

UCLASS()
class LITTLERPG_API ALittlePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALittlePlayerState();

	UFUNCTION(BlueprintCallable)
	ULittleInventoryManagerComponent* GetInventoryManager() const { return InventoryManager; }
	
	UFUNCTION(BlueprintCallable)
	ULittleCraftManagerComponent* GetCraftManager() const { return CraftManager; }



protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ULittleInventoryManagerComponent> InventoryManager;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ULittleCraftManagerComponent> CraftManager;
};
