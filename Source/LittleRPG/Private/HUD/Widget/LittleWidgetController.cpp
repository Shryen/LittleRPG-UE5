#include "HUD/Widget/LittleWidgetController.h"

#include "HUD/Widget/LittleUserWidget.h"

void ULittleWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	AbilitySystemComponent = Params.AbilitySystemComponent;
	AttributeSet = Params.AttributeSet;
}

void ULittleWidgetController::SetWidget(ULittleUserWidget* InWidget)
{
	OwnerWidget = InWidget;
}
