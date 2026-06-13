#include "HUD/Widget/LittleWidgetController.h"

void ULittleWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& Params)
{
	PlayerController = Params.PlayerController;
	PlayerState = Params.PlayerState;
	StatComponent = Params.StatComponent;
}
