#include "HUD/Widget/LittleUserWidget.h"

void ULittleUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	ControllerSet();
}
