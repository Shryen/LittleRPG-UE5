#pragma once

#include "CoreMinimal.h"
#include "HUD/Widget/LittleWidgetController.h"
#include "CraftingWidgetController.generated.h"


class UCraftingWidget;

UCLASS()
class LITTLERPG_API UCraftingWidgetController : public ULittleWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void SetWidget(ULittleUserWidget* InWidget) override;
	void BindDependencies();
	
	void CraftItem(FName ItemRowName);
	
private:
	UPROPERTY()
	TObjectPtr<UCraftingWidget> CraftingWidget;
};
