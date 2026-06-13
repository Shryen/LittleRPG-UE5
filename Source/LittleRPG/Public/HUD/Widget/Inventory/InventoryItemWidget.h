#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryItemWidget.generated.h"

class UTextBlock;
class UImage;
class UItemData;

UCLASS()
class LITTLERPG_API UInventoryItemWidget : public ULittleUserWidget
{
	GENERATED_BODY()
public:
	void SetItemData(const UItemData* Item);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
};