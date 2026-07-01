#include "HUD/Widget/Stats/StatLineWidget.h"
#include "Components/TextBlock.h"

void UStatLineWidget::SetValues(const FName& AttributeName, int32 Value)
{
	if (Attribute)
		Attribute->SetText(FText::FromName(AttributeName));

	if (AttributeValue)
		AttributeValue->SetText(FText::AsNumber(Value));
}
