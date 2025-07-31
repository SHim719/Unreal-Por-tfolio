// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/RPGAbility_CombatAbility.h"
#include "RPGAbility_MutantGroundSmash.generated.h"


class UAbilityTask_Tick;
class ARPGRadialIndicator;
class UAbilityTask_ApplyRootMotionJumpForce;

UCLASS()
class RPGPROJECT_API URPGAbility_MutantGroundSmash : public URPGAbility_CombatAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	void SettingWarningIndicator();
	void WaitSmashAttack(float DeltaTime);

	UFUNCTION()
	void SmashAttack(FGameplayEventData EventData);
	

protected:
	UPROPERTY(EditDefaultsOnly)
	float SmashWaitTime = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 0.f;

	UPROPERTY(EditDefaultsOnly)
	FVector HitQueryLocalOffset;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARPGRadialIndicator> IndicatorClass;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameplayCueTag;
	
protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitHitEventTask;

	UPROPERTY()
	TObjectPtr<UAbilityTask_Tick> TickTask;

	UPROPERTY()
	TObjectPtr<ARPGRadialIndicator> WarningIndicator;

	float CurrentTime = 0.f;
	
};
