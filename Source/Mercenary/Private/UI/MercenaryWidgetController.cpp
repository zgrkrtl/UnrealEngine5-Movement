// Mercenary Game


#include "UI/MercenaryWidgetController.h"

void UMercenaryWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UMercenaryWidgetController::BroadcastInitialValues()
{
}

void UMercenaryWidgetController::BindCallbacksToDependencies()
{
}
