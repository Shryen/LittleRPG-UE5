#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LittlePlayerState.generated.h"

class ULittleInventoryManagerComponent;

UCLASS()
class LITTLERPG_API ALittlePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	ALittlePlayerState();

	UFUNCTION(BlueprintCallable)
	ULittleInventoryManagerComponent* GetInventoryManager() const { return InventoryManager; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ULittleInventoryManagerComponent> InventoryManager;
};
