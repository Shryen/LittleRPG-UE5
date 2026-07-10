#include "HUD/Widget/LittleWidgetController.h"

#include "HUD/Widget/LittleUserWidget.h"

void ULittleWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
}

void ULittleWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	OwnerWidget = InWidget;
}
