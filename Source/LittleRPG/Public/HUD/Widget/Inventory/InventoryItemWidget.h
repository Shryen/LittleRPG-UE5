#pragma once

#include "CoreMinimal.h"
#include "Data/FInventorySlot.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryItemWidget.generated.h"

struct FItemDataRow;
class UDataTable;
class UTextBlock;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnInventoryItemClicked, int32 /*SlotID*/)

UCLASS()
class LITTLERPG_API UInventoryItemWidget : public ULittleUserWidget
{
	GENERATED_BODY()
public:
	
	FOnInventoryItemClicked OnItemClicked;
	
	void SetItemRow(const FItemDataRow* Row, int32 Quantity);
	void UpdateQuantity(const FInventorySlot& InventorySlot);
	void InitSlot(const int32 SlotIndex);
	void UpdateSlot(const FInventorySlot& InventorySlot);

	UPROPERTY()
	TObjectPtr<UDataTable> ItemDataTable;
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	int32 CachedSlotID = INDEX_NONE;
};