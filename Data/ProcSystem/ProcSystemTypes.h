#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "ProcSystemTypes.generated.h"

class UGameplayAbility;
class UGameplayEffect;

UENUM()
enum class ERPGProcTarget
{
	Self,
	Target,
};


USTRUCT()
struct FRPGProcEffectDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag  TriggerTag;

	UPROPERTY(EditDefaultsOnly)
	ERPGProcTarget Target;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectToApply;
	
};

USTRUCT()
struct FRPGProcEffectInstance
{
	GENERATED_BODY()

	FGameplayTag TriggerTag;
	ERPGProcTarget Target;
	float ChancePercentage;
	
	UPROPERTY()
	FGameplayEffectSpecHandle ApplyEffectSpecHandle;

	bool operator==(const FRPGProcEffectInstance& Other) const
	{
		return  ApplyEffectSpecHandle == Other.ApplyEffectSpecHandle;
	}
};