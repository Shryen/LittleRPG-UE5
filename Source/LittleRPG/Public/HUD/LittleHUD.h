#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widget/LittleWidgetController.h"
#include "LittleHUD.generated.h"


class UAbilityBarWidgetController;
class UCraftingWidgetController;
class UProgressBarWidgetController;
class UMainLayoutWidget;
class UMainLayoutWidgetController;
class UInventoryWidgetController;
class ULittleWidgetController;
class ULittleUserWidget;
class UInventoryWidget;

UCLASS()
class LITTLERPG_API ALittleHUD : public AHUD
{
	GENERATED_BODY()

public:
	UInventoryWidgetController* GetInventoryWidgetController() const { return InventoryWidgetController; }
	UProgressBarWidgetController* GetHealthBarWidgetController() const { return HealthBarWidgetController; }
	UProgressBarWidgetController* GetStaminaBarWidgetController() const { return StaminaBarWidgetController; }
	UMainLayoutWidgetController* GetMainLayoutWidgetController() const { return MainLayoutWidgetController; }
	UCraftingWidgetController* GetCraftingWidgetController() const { return CraftingWidgetController; }
	UAbilityBarWidgetController* GetAbilityBarController() const { return AbilityBarController; }
	
protected:
	bool SetupMainOverlayWidget(APlayerController* PC);
	void SetupMainLayoutWidgetController(FWidgetControllerParams Params);
	void SetupInventoryWidgetController(FWidgetControllerParams Params);
	void SetupHealthBarWidgetController(FWidgetControllerParams Params);
	void SetupStaminaBarWidgetController(FWidgetControllerParams Params);
	void SetupCraftingWidgetController(FWidgetControllerParams Params);
	void SetupAbilityBarController(FWidgetControllerParams Params);
	void SetupPlayerStateDependencies();
	virtual void BeginPlay() override;
	void OnPlayerStateReady();
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULittleUserWidget> MainOverlayWidgetClass;

private:
	UPROPERTY()
	TObjectPtr<UMainLayoutWidgetController> MainLayoutWidgetController;
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	UPROPERTY()
	TObjectPtr<UProgressBarWidgetController> HealthBarWidgetController;
	UPROPERTY()
	TObjectPtr<UProgressBarWidgetController> StaminaBarWidgetController;
	UPROPERTY()
	TObjectPtr<UCraftingWidgetController> CraftingWidgetController;
	UPROPERTY()
	TObjectPtr<UAbilityBarWidgetController> AbilityBarController;
	
	UPROPERTY()
	TObjectPtr<UMainLayoutWidget> MainOverlayWidget;
};
