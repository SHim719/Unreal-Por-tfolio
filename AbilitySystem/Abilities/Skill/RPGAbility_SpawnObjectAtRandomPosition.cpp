// 


#include "RPGAbility_SpawnObjectAtRandomPosition.h"

#include "NavigationSystem.h"
#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Actor/EffectActor/RPGEffectActor.h"
#include "ObjectPool/RPGPoolManager.h"


void URPGAbility_SpawnObjectAtRandomPosition::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                              const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                              const FGameplayEventData* TriggerEventData)
{

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::SpawnObjects),
		SpawnPerSec, true);
	
}

void URPGAbility_SpawnObjectAtRandomPosition::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	GetWorld()->GetTimerManager().ClearTimer(SpawnTimerHandle);
	SpawnTimerHandle.Invalidate();
}




void URPGAbility_SpawnObjectAtRandomPosition::SpawnObjects()
{
	auto* PoolManager = RPGHelper::GetPoolManager(this);
	if (PoolManager == nullptr)
		return;

	TArray<FGameplayEffectSpecHandle> EffectSpecHandles;
	GetEffectSpecHandles(EffectSpecHandles);
	
	float DeltaAngle = 360.f / SpawnCountAtOnce;
	for (int32 i = 0; i < SpawnCountAtOnce; ++i)
	{
		const float RandAngle = FMath::DegreesToRadians(FMath::RandRange(DeltaAngle * i ,DeltaAngle * (i + 1)));
		const float RandDist = FMath::RandRange(SpawnMinRadius, SpawnMaxRadius);
		
		FVector SpawnLocation = RPGHelper::GetActorFootLocation(GetOwningActorFromActorInfo());
		SpawnLocation += FVector(FMath::Cos(RandAngle), FMath::Sin(RandAngle), 0.f) * RandDist;

		if (ARPGEffectActor* SpawnActor = Cast<ARPGEffectActor>(PoolManager->GetPoolObject(ObjectPoolKey)))
		{
			SpawnActor->SetEffectSpecHandles(EffectSpecHandles);
			SpawnActor->SetActorLocation(SpawnLocation);
			SpawnActor->Initialize();
		}
	}
	
}
