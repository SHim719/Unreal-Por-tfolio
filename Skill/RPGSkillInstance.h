

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpec.h"
#include "GameplayTagContainer.h"
#include "RPGSkillInstance.generated.h"


struct FRPGSkillTreeNode;
DECLARE_DELEGATE_OneParam(FOnUpdateSkillEntryWidget, const int32)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnCooldownChanged, float TimeRemaining, float CooldownTime, const int32 EnrolledQuickSlotIndex);

class URPGAbilitySystemComponent;

UCLASS()
class RPGPROJECT_API URPGSkillInstance : public UObject
{
	GENERATED_BODY()
	
public:
	void Init(const FGameplayTag& SkillTag, URPGAbilitySystemComponent* InOwnerASC);
	
	void OnInputPressed();
	void OnInputReleased();

	int32 GetCurrentSkillLevel() const;
	float GetCurrentCooldown() const;
	FGameplayTag GetSkillTag() const { return RefSkillTag; }
	const FRPGSkillTreeNode* GetSkillTreeNode() const;
	
private:
	void InitCooldown(const FGameplayTag& InCooldownTag);
	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void UpdateCooldownTimeRemaining();


public:
	FOnUpdateSkillEntryWidget OnUpdateSkillEntryWidget;
	FOnCooldownChanged OnCooldownChanged;
	
	int32 EnrolledQuickSlotIndex = 0;
	
private:
	UPROPERTY()
	TObjectPtr<URPGAbilitySystemComponent> OwnerASC;
	
	FGameplayTag RefSkillTag = FGameplayTag::EmptyTag;
	FGameplayTag CooldownTag = FGameplayTag::EmptyTag;

	FTimerHandle CooldownUpdateTimerHandle;
	float CachedCooldownTime = -1.f;
};
