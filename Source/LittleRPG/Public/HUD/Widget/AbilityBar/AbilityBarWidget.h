#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "AbilityBarWidget.generated.h"

class UHorizontalBox;
/**
 * 
 */
UCLASS()
class LITTLERPG_API UAbilityBarWidget : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite)
	TObjectPtr<UHorizontalBox> AbilityBar;
};
