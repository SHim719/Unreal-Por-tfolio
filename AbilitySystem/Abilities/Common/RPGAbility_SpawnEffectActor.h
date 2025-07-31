// 

#pragma once

#include "CoreMinimal.h"
#include "RPGAbility_CombatAbility.h"
#include "RPGAbility_SpawnEffectActor.generated.h"

class ARPGEffectActor;
/**
 * 
 */
UCLASS()
class RPGPROJECT_API URPGAbility_SpawnEffectActor : public URPGAbility_CombatAbility
{
	GENERATED_BODY()

public:
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	

protected:
	virtual void InitObjectPool();
	virtual void OnChangeAbilityLevel(const FGameplayTag& Tag, const int32 NewLevel);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARPGEffectActor> EffectActorClassToSpawn;

	UPROPERTY(EditDefaultsOnly)
	FString ObjectPoolKey;
	
	UPROPERTY(EditDefaultsOnly)
	int32 InitPoolSize = 0;
	
};
