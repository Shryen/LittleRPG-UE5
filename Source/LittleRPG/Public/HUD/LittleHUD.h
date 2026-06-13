#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LittleHUD.generated.h"


class UMainLayoutWidget;
class UInventoryWidgetController;
class ULittleWidgetController;
class ULittleUserWidget;
class UHealthWidget;
class UInventoryWidget;

UCLASS()
class LITTLERPG_API ALittleHUD : public AHUD
{
	GENERATED_BODY()

public:
	UInventoryWidgetController* GetInventoryWidgetController() const { return InventoryWidgetController; }

	
protected:
	virtual void BeginPlay() override;
	void OnPlayerStateReady();
	
	UFUNCTION()
	void OnHealthChanged(float CurrentHealth);

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UHealthWidget> HealthBarWidgetClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULittleUserWidget> MainOverlayWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	UPROPERTY()
	TObjectPtr<UHealthWidget> HealthBarWidget;
	
	UPROPERTY()
	TObjectPtr<UMainLayoutWidget> MainOverlayWidget;
	
};
