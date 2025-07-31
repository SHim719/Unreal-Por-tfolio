#include "RPGConsumeAttribute_ApplyEffect.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"
#include "Data/Item/RPGItemOption.h"

bool URPGConsumeAttribute_ApplyEffect::Consume(AActor* Player)
{
	if (Player == nullptr)
		return false;

	auto* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Player);
	if (ASC == nullptr)
		return false;
	
	const FGameplayEffectContextHandle EffectContext = ASC->MakeEffectContext();
	FGameplayEffectSpecHandle EffectSpecHandle = ASC->MakeOutgoingSpec(GameplayEffect, 1.f, EffectContext);
	FGameplayEffectSpec* Spec = EffectSpecHandle.Data.Get();
	if (Spec)
	{
		for (const TPair<FGameplayTag, int32>& AttributeData : SetByCallerValues)
		{
			Spec->SetSetByCallerMagnitude(AttributeData.Key, AttributeData.Value);
		}

		if (bHasDuration)
		{
			Spec->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Effect.Duration")), Duration);
			Spec->Period = Period;
			Spec->SetStackCount(StackCount);
		}
		
		ASC->ApplyGameplayEffectSpecToSelf(*Spec);
	}

	PlayConsumeSound(Player);
	
	return true;
}

FText URPGConsumeAttribute_ApplyEffect::GetTooltipText(const UObject* WorldContextObject)
{
	if (OptionRowHandle.IsNull())
		return FText::GetEmpty();
	
	const FRPGItemOptionRow* ItemOptionData = OptionRowHandle.GetRow<FRPGItemOptionRow>(TEXT("Query ItemOption"));
	if (ItemOptionData == nullptr)
		return FText::GetEmpty();
	
	FFormatNamedArguments Arguments;
		
	for (const TPair<FGameplayTag, int32>& Pair : SetByCallerValues)
	{
		FString KeyString = Pair.Key.GetTagName().ToString();
		FText ValueText = FText::AsNumber(Pair.Value);
		Arguments.Add(KeyString, ValueText);
	}
	
	const FText FinalText = FText::Format(ItemOptionData->OptionText, Arguments);
	return FinalText;
}