#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "CraftingSlotWidget.generated.h"

class UImage;
class UTextBlock;
struct FCraftSlot;

UCLASS()
class LITTLERPG_API UCraftingSlotWidget : public ULittleUserWidget
{
	GENERATED_BODY()

public:
	void UpdateSlot(const FCraftSlot& Slot);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> SlotName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> SlotIcon;
};
