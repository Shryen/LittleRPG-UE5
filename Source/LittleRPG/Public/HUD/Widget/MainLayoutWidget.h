
#pragma once

#include "CoreMinimal.h"
#include "LittleUserWidget.h"
#include "MainLayoutWidget.generated.h"

class UHealthWidget;
class UInventoryWidget;

UCLASS()
class LITTLERPG_API UMainLayoutWidget : public ULittleUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UInventoryWidget> InventoryWidget;
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UHealthWidget> HealthBarWidget;
};
