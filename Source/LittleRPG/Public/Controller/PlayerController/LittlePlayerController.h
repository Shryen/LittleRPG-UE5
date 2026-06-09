#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LittlePlayerController.generated.h"

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

public:
	ALittlePlayerController();

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void HandleInventory(const FInputActionValue& Value);
	
	void TestDamage();

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UInputMappingContext> MovementMappingContext;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> InventoryAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> TakeDamageTest;
	
};
