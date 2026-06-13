
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LittleUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LITTLERPG_API ULittleUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void SetWidgetController(UObject* InWidgetController);
	
	UPROPERTY(BlueprintReadOnly, Category = "LittleRPG|Widget|Controller")
	TObjectPtr<UObject> WidgetController;
}; 
