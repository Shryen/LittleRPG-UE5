#pragma once

#include "CoreMinimal.h"
#include "Data/FInventorySlot.h"
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
	void UpdateQuantity(const FInventorySlot& InventorySlot);
	void InitSlot(const FInventorySlot& InventorySlot);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	int32 CachedSlotID = INDEX_NONE;
};