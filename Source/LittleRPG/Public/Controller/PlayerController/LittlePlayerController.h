#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/PlayerController.h"
#include "LittlePlayerController.generated.h"

class ALittleBaseCharacter;
class ULittleAbilitySystemComponent;
class AResourceNode;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class LITTLERPG_API ALittlePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	
	UFUNCTION(Server, Reliable)
	void Server_Interact(AActor* InteractableActor);
	
	UFUNCTION(Server, Reliable)
	void Server_InteractAndSwap(UInstancedStaticMeshComponent* Component, int32 InstanceIndex);

	UFUNCTION(Exec)
	void CheatAddItem(FName ItemRowName, int32 Quantity = 1);
	
	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	TMap<FGameplayTag, TSubclassOf<AResourceNode>> ResourceNodeClassMap;

private:
	UPROPERTY()
	TObjectPtr<ULittleAbilitySystemComponent> LittleASC;
	
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump(const FInputActionValue& Value);
	void StopJump(const FInputActionValue& Value);
	void HandleInventory(const FInputActionValue& Value);
	void HandleInteract(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MovementMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Little|Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Little|Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|Input")
	TObjectPtr<UInputAction> JumpAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|Input")
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|Input")
	TObjectPtr<UInputAction> InteractAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Little|Input")
	TObjectPtr<UInputAction> AttackAction;
};
