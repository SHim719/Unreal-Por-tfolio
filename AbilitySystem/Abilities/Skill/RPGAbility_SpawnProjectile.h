// 

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/Common/RPGAbility_SpawnEffectActor.h"
#include "RPGAbility_SpawnProjectile.generated.h"


class UAbilityTask_WaitGameplayEvent;
class ARPGProjectileActor;


USTRUCT()
struct FProjectileSpawnDesc
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	FName SpawnSocketName;

	UPROPERTY(EditDefaultsOnly)
	int32 SpawnCountPerOnce;

	UPROPERTY(EditDefaultsOnly)
	FVector SpawnOffset;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="SpawnCountPerOnce > 1", EditConditionHides))
	float SpreadAngle = 0.f;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition="SpawnCountPerOnce > 1", EditConditionHides))
	float SpawnRotationPivotAngle = 0.f;
};


UCLASS()
class RPGPROJECT_API URPGAbility_SpawnProjectile : public URPGAbility_SpawnEffectActor
{
	GENERATED_BODY()

public:
	URPGAbility_SpawnProjectile();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

protected:
	UFUNCTION(BlueprintNativeEvent)
	void SpawnProjectile(FGameplayEventData EventData);

	UFUNCTION(BlueprintNativeEvent)
	void SettingProjectile(ARPGProjectileActor* Projectile, const FTransform& SpawnTransform);
	
	FTransform GetSpawnTransform(const int32 CurSpawnCount) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<FProjectileSpawnDesc> SpawnDescs;
	
	int32 CurrentAction = 0;

protected:
	UPROPERTY()
	TObjectPtr<UAbilityTask_WaitGameplayEvent> WaitSpawnProjectileTask;
	
};
