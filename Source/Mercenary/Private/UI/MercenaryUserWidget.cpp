// Mercenary Game


#include "UI/MercenaryUserWidget.h"

void UMercenaryUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
