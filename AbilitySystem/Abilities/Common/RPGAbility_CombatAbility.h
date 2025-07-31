// 

#pragma once

#include "CoreMinimal.h"
#include "RPGDefines.h"
#include "Abilities/GameplayAbility.h"
#include "RPGAbility_CombatAbility.generated.h"


class UAbilityTask_WaitGameplayEvent;

USTRUCT()
struct FRPGGameplayEffectDef
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> EffectClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTagContainer SetByCallerTagContainer;
	
};


class UAbilityTask_PlayMontageAndWait;

UCLASS()
class RPGPROJECT_API URPGAbility_CombatAbility : public UGameplayAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual void InputReleased(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	virtual FGameplayEffectSpecHandle MakeEffectSpecHandle(const FRPGGameplayEffectDef& ApplyEffectDef) const;
	
	void GetEffectSpecHandles(OUT TArray<FGameplayEffectSpecHandle>& EffectSpecHandles) const;
	
	virtual float GetAttributeDataFromCurveTableByTag(const FGameplayTag& Tag) const;
	
	UFUNCTION()
	virtual void OnChannelingEnd();
	float GetChannelingTime() const;
	void ActivateMontageTask();
	bool IsPlayer() const;
	void LookToMouse() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimMontage>	Montage;

	UPROPERTY(EditDefaultsOnly)
	TArray<FRPGGameplayEffectDef> ApplyEffectDefs;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCurveTable>		AbilityAttributeTable;

	UPROPERTY(EditDefaultsOnly)
	bool bPlayMontageOnActivated = true;
	
	UPROPERTY(EditDefaultsOnly)
	bool bCancelOnInputReleased = false;

	UPROPERTY(EditDefaultsOnly)
	bool bIsChanneling = false;

	UPROPERTY(EditDefaultsOnly)
	bool bLookToMouse = true;

	UPROPERTY(EditDefaultsOnly)
	bool bUpdateFacingTargetOnActivated = false;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask;
	
	FGameplayTag DurationTag = FGameplayTag::RequestGameplayTag(FName("Skill.Attribute.Duration"));
	FGameplayTag PeriodTag = FGameplayTag::RequestGameplayTag(FName("Skill.Attribute.Period"));
	FGameplayTag ChannelingTimeTag = FGameplayTag::RequestGameplayTag(FName("Skill.Attribute.ChannelingTime"));

	FTimerHandle ChannelingTimerHandle;
};
