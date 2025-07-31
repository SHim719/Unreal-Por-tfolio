// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/RPGAbility_SpawnEffectActor.h"
#include "RPGAbility_SpawnObjectAtRandomPosition.generated.h"

class ARPGEffectActor;

UCLASS()
class RPGPROJECT_API URPGAbility_SpawnObjectAtRandomPosition : public URPGAbility_SpawnEffectActor
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	UFUNCTION()
	void SpawnObjects();

private:
	UPROPERTY(EditDefaultsOnly)
	int32 SpawnCountAtOnce;
	
	UPROPERTY(EditDefaultsOnly)
	float SpawnPerSec = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float SpawnMaxRadius = 0.f;
	
	UPROPERTY(EditDefaultsOnly)
	float SpawnMinRadius = 0.f;

	
	FTimerHandle SpawnTimerHandle;
};
