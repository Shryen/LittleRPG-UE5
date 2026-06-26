#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "InventoryItemWidget.generated.h"

struct FInventoryDisplayPayload;
struct FItemDataRow;
class UDataTable;
class UTextBlock;
class UImage;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnItemClicked, int32 /*SlotID*/)

UCLASS()
class LITTLERPG_API UInventoryItemWidget : public ULittleUserWidget
{
	GENERATED_BODY()
public:
	
	void UpdateFromPayload(const FInventoryDisplayPayload& Payload);
 
	int32 VisualSlotIndex = INDEX_NONE;
 
	FOnItemClicked OnItemClicked;

	UPROPERTY()
	TObjectPtr<UDataTable> ItemDataTable;
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ItemQuantity;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ItemIcon;
};