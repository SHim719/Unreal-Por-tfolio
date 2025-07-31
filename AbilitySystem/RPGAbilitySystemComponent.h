

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "RPGAbilitySystemComponent.generated.h"


class URPGGameplayAbility;
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnAbilityLevelChanged, const FGameplayTag&, const int32)

UCLASS()
class RPGPROJECT_API URPGAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void AddAbilityLevel(const FGameplayTag& AbilityTag, const int32 Add);
	void SetAbilityLevel(const FGameplayTag& AbilityTag, const int32 Level);
	int32 GetAbilityLevel(const FGameplayTag& AbilityTag) const;
	
	void AbilityInputPressedByAssetTag (const FGameplayTag& AssetTag);
	void AbilityInputReleasedByAssetTag (const FGameplayTag& AssetTag);

	UGameplayAbility* FindAbilityInstanceByAssetTag(const FGameplayTag& AssetTag);
	float GetCurrentCooldown(const FGameplayTag& AssetTag) const;

	bool IsPlayer() const;
public:
	FOnAbilityLevelChanged OnAbilityLevelChanged;
	
};
