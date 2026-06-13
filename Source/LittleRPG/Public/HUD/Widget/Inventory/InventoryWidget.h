#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryWidget.generated.h"


class UUniformGridPanel;
class UHorizontalBox;

UCLASS()
class LITTLERPG_API UInventoryWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	void AddItem(const FText& ItemName);
	void ClearItems();

protected:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHorizontalBox> InventoryContainerBox;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UUniformGridPanel> InventoryGridPanel;
};
