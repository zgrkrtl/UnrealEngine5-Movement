// Mercenary Game


#include "Player/MercenaryPlayerState.h"
#include "AbilitySystem/MercenaryASC.h"
#include "AbilitySystem/MercenaryAS.h"

AMercenaryPlayerState::AMercenaryPlayerState()
{
	//initialize AbilitySystemComponent and AttributeSet
	AbilitySystemComponent = CreateDefaultSubobject<UMercenaryASC>("AbilitySystemComponent");
	AttributeSet = CreateDefaultSubobject<UMercenaryAS>("AttributeSet");
}

UAbilitySystemComponent* AMercenaryPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
