


#include "AbilitySystem/RPGAbilitySystemGlobals.h"
#include "AbilitySystem/RPGAbilityTypes.h"

FGameplayEffectContext* URPGAbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FRPGGameplayEffectContext();
}
