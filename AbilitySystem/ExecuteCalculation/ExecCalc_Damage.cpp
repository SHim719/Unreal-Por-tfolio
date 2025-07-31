#include "AbilitySystem/ExecuteCalculation/ExecCalc_Damage.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "AbilitySystem/RPGAbilityTypes.h"
#include "AbilitySystem/RPGAttributeSet.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(ExecCalc_Damage)


struct RPGDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Attack);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalRate);

	RPGDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Defense, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, Attack, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(URPGAttributeSet, CriticalRate, Source, false);
	}
};


static const RPGDamageStatics& DamageStatics()
{
	static RPGDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackDef);
	RelevantAttributesToCapture.Add(DamageStatics().DefenseDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalRateDef);
}


void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams,
                                              FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	auto* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	auto* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	
	if (IsTargetInvincible(Cast<URPGAbilitySystemComponent>(TargetASC)) == true)
		return;
	
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	float BaseDamage = EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Skill.Attribute.BaseDamage")));
	float AttackCoefficient = EffectSpec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Skill.Attribute.AttackCoefficient")));
	
	FAggregatorEvaluateParameters EvaluationParameters;
	
	float SourceAttack = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().AttackDef, EvaluationParameters, SourceAttack);

	float SourceCriticalRate = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalRateDef, EvaluationParameters, SourceCriticalRate);

	float TargetDefense = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().DefenseDef, EvaluationParameters, TargetDefense);
	
	float FinalDamage = BaseDamage * SourceAttack * AttackCoefficient + SourceAttack * AttackCoefficient - TargetDefense;

	if (IsCritical(SourceCriticalRate) == true)
	{
		FGameplayEffectContextHandle EffectContextHandle = EffectSpec.GetContext();
		FRPGGameplayEffectContext* EffectContext = static_cast<FRPGGameplayEffectContext*>(EffectContextHandle.Get());
		if (EffectContext)
		{
			EffectContext->SetIsCriticalHit(true);
		}
		
		FinalDamage *= 1.5f;
	}
	
	const FGameplayModifierEvaluatedData EvaluatedData(URPGAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, FinalDamage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
	
	SendEventToASC(SourceASC, TargetASC
		, FGameplayTag::RequestGameplayTag(FName("GameEvent.OnHit")), FinalDamage);
	SendEventToASC(SourceASC, SourceASC,
		FGameplayTag::RequestGameplayTag(FName("GameEvent.OnAttack")), FinalDamage);
	
}

bool UExecCalc_Damage::IsTargetInvincible(UAbilitySystemComponent* TargetASC) const
{
	if (TargetASC == nullptr)
		return true;

	if (TargetASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(FName("State.Invincible"))) == true)
		return true;

	return false;
}

bool UExecCalc_Damage::IsCritical(const float CriticalRate) const
{
	if (FMath::RandRange(0.f, 1.f) < (CriticalRate / 100.f))
		return true;

	return false;
}

void UExecCalc_Damage::SendEventToASC(UAbilitySystemComponent* SourceASC, UAbilitySystemComponent* TargetASC,
	FGameplayTag EventTag, float Value) const
{
	if (SourceASC == nullptr || TargetASC == nullptr)
		return;
	
	FGameplayEventData EventData;
	EventData.EventTag = EventTag;
	EventData.Target = TargetASC->GetOwnerActor();
	EventData.EventMagnitude = Value;
	EventData.Instigator = SourceASC->GetOwnerActor();

	TargetASC->HandleGameplayEvent(EventData.EventTag, &EventData);
}



