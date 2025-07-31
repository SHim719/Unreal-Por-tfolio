#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Abilities/GameplayAbility.h"


void URPGAbilitySystemComponent::AddAbilityLevel(const FGameplayTag& AbilityTag, const int32 Add) 
{
	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(AbilityTag))
		{
			AbilitySpec.Level += Add;
			OnAbilityLevelChanged.Broadcast(AbilityTag, AbilitySpec.Level);
			break;
		}
	}
}

void URPGAbilitySystemComponent::SetAbilityLevel(const FGameplayTag& AbilityTag, const int32 Level)
{
	for (FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(AbilityTag))
		{
			if (AbilitySpec.Level != Level)
			{
				AbilitySpec.Level = Level;
				OnAbilityLevelChanged.Broadcast(AbilityTag, Level);
			}
			break;
		}
	}
}

int32 URPGAbilitySystemComponent::GetAbilityLevel(const FGameplayTag& AbilityTag) const
{
	for (const FGameplayAbilitySpec& AbilitySpec : ActivatableAbilities.Items)
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(AbilityTag))
		{
			return AbilitySpec.Level;
		}
	}

	return 0;
}

void URPGAbilitySystemComponent::AbilityInputPressedByAssetTag(const FGameplayTag& AssetTag)
{
	if (!AssetTag.IsValid()) return;
	
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(AssetTag))
		{
			AbilitySpecInputPressed(AbilitySpec);
			if (!AbilitySpec.IsActive())
			{
				TryActivateAbility(AbilitySpec.Handle);
			}
		}
	}
}

void URPGAbilitySystemComponent::AbilityInputReleasedByAssetTag(const FGameplayTag& AssetTag)
{
	if (!AssetTag.IsValid()) return;
 
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(AssetTag))
		{
			AbilitySpecInputReleased(AbilitySpec);
		}
	}
}

UGameplayAbility* URPGAbilitySystemComponent::FindAbilityInstanceByAssetTag(const FGameplayTag& AssetTag)
{
	if (!AssetTag.IsValid())
		return nullptr;
 
	for (FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability->GetAssetTags().HasTagExact(AssetTag))
		{
			return AbilitySpec.Ability;
		}
	}

	return nullptr;
}

float URPGAbilitySystemComponent::GetCurrentCooldown(const FGameplayTag& AssetTag) const
{
	float Cooldown = 0.f;
	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		if (AbilitySpec.Ability && AbilitySpec.Ability->GetAssetTags().HasTagExact(AssetTag))
		{
			if (auto* CooldownEffect = AbilitySpec.Ability->GetCooldownGameplayEffect())
			{
				CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(AbilitySpec.Level, Cooldown);
			}

			return Cooldown;
		}
	}
	
	return Cooldown;
}

bool URPGAbilitySystemComponent::IsPlayer() const
{
	if (AActor* OwningActor = GetOwnerActor())
	{
		return OwningActor->ActorHasTag(FName("Player"));
	}
	return false;
}






