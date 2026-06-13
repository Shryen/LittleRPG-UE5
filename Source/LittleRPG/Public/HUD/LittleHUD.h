#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Widget/LittleWidgetController.h"
#include "LittleHUD.generated.h"


class UHealthBarWidgetController;
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
	UHealthBarWidgetController* GetHealthBarWidgetController() const { return HealthBarWidgetController; }
	
protected:
	bool SetupMainOverlayWidget(APlayerController* PC);
	void SetupInventoryWidgetController(FWidgetControllerParams Params);
	void SetupHealthBarWidgetController(FWidgetControllerParams Params);
	void SetupPlayerStateDependencies();
	virtual void BeginPlay() override;
	void OnPlayerStateReady();
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<ULittleUserWidget> MainOverlayWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryWidgetController> InventoryWidgetController;
	
	UPROPERTY()
	TObjectPtr<UHealthBarWidgetController> HealthBarWidgetController;
	
	UPROPERTY()
	TObjectPtr<UMainLayoutWidget> MainOverlayWidget;
};
