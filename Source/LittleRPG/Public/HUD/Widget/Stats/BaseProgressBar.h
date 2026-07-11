#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleUserWidget.h"
#include "BaseProgressBar.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS()
class LITTLERPG_API UBaseProgressBar : public ULittleUserWidget
{
	GENERATED_BODY()
	
public:
	void UpdateProgressBar(float Value, float MaxValue) const;
	
	UPROPERTY(meta=(BindWidget), BlueprintReadWrite, Category = "ProgressBar")
	TObjectPtr<UProgressBar> ProgressBar;
};
