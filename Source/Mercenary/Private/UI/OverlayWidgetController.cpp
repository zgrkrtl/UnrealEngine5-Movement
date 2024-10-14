// Mercenary Game


#include "UI/OverlayWidgetController.h"
#include "AbilitySystem/MercenaryAS.h"
#include "AbilitySystem/MercenaryASC.h"

void UOverlayWidgetController::BroadcastInitialValues()
{
	const UMercenaryAS* MercenaryAS = CastChecked<UMercenaryAS>(AttributeSet);

	OnHealthChanged.Broadcast(MercenaryAS->GetHealth());
	OnMaxHealthChanged.Broadcast(MercenaryAS->GetMaxHealth());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UMercenaryAS* MercenaryAS = CastChecked<UMercenaryAS>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MercenaryAS->GetHealthAttribute()).AddUObject(this, &ThisClass::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(MercenaryAS->GetMaxHealthAttribute()).AddUObject(this, &ThisClass::MaxHealthChanged);

}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);

}
