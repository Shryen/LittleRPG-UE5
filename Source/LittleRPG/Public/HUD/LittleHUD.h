#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LittleHUD.generated.h"


class UInventoryWidget;

UCLASS()
class LITTLERPG_API ALittleHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void ToggleInventory();
	
	bool bInventoryOpen = false;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<UInventoryWidget> InventoryWidgetClass;
	
private:
	UPROPERTY()
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	void ShowInventoryWidget();
	void HideInventoryWidget();
};
