#pragma once

#include "CoreMinimal.h"
#include "Data/Equipment/EquipmentSlot.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "EquipmentSlotWidget.generated.h"

struct FEquipmentDisplayPayload;
enum class EEquipmentSlot : uint8;
class UBorder;
class UImage;
/**
 * 
 */

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEquipmentSlotRightClicked, EEquipmentSlot);

UCLASS()
class LITTLERPG_API UEquipmentSlotWidget : public ULittleUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LittleRPG|Equipment")
	EEquipmentSlot SlotType = EEquipmentSlot::Empty;
	
	void UpdateFromPayload(const FEquipmentDisplayPayload& Payload);

	FOnEquipmentSlotRightClicked OnEquipmentSlotRightClicked;
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UImage> ItemIcon;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UBorder> BackgroundBorder;
	
	bool bIsOccupied = false;
};
