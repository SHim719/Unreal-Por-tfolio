// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/RPGAbility_CombatAbility.h"
#include "RPGAbility_SpawnAtCircle.generated.h"


class ARPGEffectActor;
class ARPGTargetCircle;

UCLASS()
class RPGPROJECT_API URPGAbility_SpawnAtCircle : public URPGAbility_CombatAbility
{
	GENERATED_BODY()

public:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void InputPressed(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

protected:
	virtual void SpawnActorAtPosition(bool bCanceled, const FVector& SpawnPosition);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float RangeRadius;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARPGEffectActor> SpawnActorClass;

};
