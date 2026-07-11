
#pragma once

#include "CoreMinimal.h"
#include "LittleUserWidget.h"
#include "MainLayoutWidget.generated.h"

class UVitalBars;
class UBaseProgressBar;
class UButton;
class UOverlay;
class UWidgetSwitcher;
class UInventoryWidget;
class UCraftingWidget;

UCLASS()
class LITTLERPG_API UMainLayoutWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UOverlay> InventoryOverlay;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UWidgetSwitcher> WidgetSwitcher;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> InventoryButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> CraftingButton;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UCraftingWidget> CraftingWidget;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UVitalBars> VitalBarsWidget;
};
