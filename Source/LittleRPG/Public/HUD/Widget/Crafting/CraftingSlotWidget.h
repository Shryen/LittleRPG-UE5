#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "CraftingSlotWidget.generated.h"

class UImage;
class UTextBlock;
struct FCraftSlot;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCraftingSlotClicked, FName /* ItemRowName */);

UCLASS()
class LITTLERPG_API UCraftingSlotWidget : public ULittleUserWidget
{
	GENERATED_BODY()

public:
	void InitSlot(const FName& InItemRowName);
	void UpdateSlot(const FCraftSlot& Slot);
	
	FName GetItemRowName() const {return ItemRowName;} // not using const FName& because of the nature of FName it is just a lightweight ID (an 8-byte integer)
	
	FOnCraftingSlotClicked OnSlotClicked;
	
protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotIcon;
	
	UPROPERTY(EditAnywhere, Category = "Item")
	FName ItemRowName;
};
