

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "RPGAbilitySystemGlobals.generated.h"


UCLASS()
class RPGPROJECT_API URPGAbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()
	
	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
