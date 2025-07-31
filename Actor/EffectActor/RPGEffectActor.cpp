// 


#include "RPGEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "GameplayEffect.h"
#include "RPGHelperLibrary.h"


ARPGEffectActor::ARPGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ARPGEffectActor::Initialize()
{
}

void ARPGEffectActor::SetEffectSpecHandles(const TArray<FGameplayEffectSpecHandle>& InEffectSpecHandles)
{
	EffectSpecHandles = InEffectSpecHandles;
}


bool ARPGEffectActor::ApplyEffectToTarget(AActor* OtherActor)
{
	if (OtherActor == nullptr)
		return false;

	if (RPGHelper::IsFriend(OtherActor, GetEffectCauser()) == true)
		return false;

	if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor))
	{
		for (auto& DamageEffectSpecHandle : EffectSpecHandles)
		{
			if (DamageEffectSpecHandle.IsValid())
			{
				TargetASC->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
			}
		}
	}
	return true;
}

AActor* ARPGEffectActor::GetEffectCauser() const
{
	for (auto& DamageEffectSpecHandle : EffectSpecHandles)
	{
		if (DamageEffectSpecHandle.IsValid())
		{
			if (FGameplayEffectSpec* Spec = DamageEffectSpecHandle.Data.Get())
			{
				AActor* EffectCauser = Spec->GetContext().GetEffectCauser();
				return EffectCauser;
			}
		}
	}
	
	return nullptr;
}



