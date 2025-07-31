

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ExecCalc_Damage.generated.h"


class URPGAbilitySystemComponent;

UCLASS()
class RPGPROJECT_API UExecCalc_Damage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UExecCalc_Damage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

private:
	bool IsTargetInvincible(UAbilitySystemComponent* TargetASC) const;
	bool IsCritical(const float CriticalRate) const;
	void SendEventToASC(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC, FGameplayTag EventTag, float Value) const;
};
