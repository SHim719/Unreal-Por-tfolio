

#pragma once

#include "CoreMinimal.h"
#include "RPGDefines.h"
#include "AbilitySystem/Abilities/Common/RPGAbility_CombatAbility.h"
#include "RPGAbility_MeleeSkill.generated.h"


class UAbilityTask_WaitGameplayEvent;

UCLASS()
class RPGPROJECT_API URPGAbility_MeleeSkill : public URPGAbility_CombatAbility
{
	GENERATED_BODY()

public:
	URPGAbility_MeleeSkill();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)	override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)	override;

	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	
	UFUNCTION()
	void SetEnableNextCombo(FGameplayEventData EventData);

	UFUNCTION(BlueprintNativeEvent)
	void QueryDamage(FGameplayEventData EventData);

	virtual float GetAttributeDataFromCurveTableByTag(const FGameplayTag& Tag) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FHitQueryDefine>			HitQueryDefines;
	
	UPROPERTY(EditDefaultsOnly)
	bool bInputCombo = false;
	
	UPROPERTY(EditDefaultsOnly)
	int32 MaxComboCount = 0;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> QueryDamageEventTask;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitEnableNextSkillTask;
	
	int32 CurComboCount = 0;
	bool bCanNextCombo = false;
};
