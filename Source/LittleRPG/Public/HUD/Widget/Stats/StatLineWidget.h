#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "StatLineWidget.generated.h"

class UTextBlock;

UCLASS()
class LITTLERPG_API UStatLineWidget : public ULittleUserWidget
{
	GENERATED_BODY()

public:
	void SetValues(const FName& AttributeName, int32 Value);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Attribute;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AttributeValue;
};
