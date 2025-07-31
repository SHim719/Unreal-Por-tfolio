// 


#include "RPGAbility_SpawnProjectile.h"

#include "RPGHelperLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Actor/EffectActor/RPGProjectileActor.h"
#include "ObjectPool/RPGPoolManager.h"
#include "Character/RPGCharacterBase.h"

URPGAbility_SpawnProjectile::URPGAbility_SpawnProjectile()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}



void URPGAbility_SpawnProjectile::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	WaitSpawnProjectileTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, FGameplayTag::RequestGameplayTag(FName("Combat.SpawnProjectile")), nullptr, false, true);
	if (WaitSpawnProjectileTask)
	{
		WaitSpawnProjectileTask->EventReceived.AddDynamic(this, &ThisClass::SpawnProjectile);
		WaitSpawnProjectileTask->ReadyForActivation();
	}

	CurrentAction = 0;
	
}


void URPGAbility_SpawnProjectile::SpawnProjectile_Implementation(FGameplayEventData EventData)
{
	URPGPoolManager* PoolManager = RPGHelper::GetPoolManager(this);
	if (PoolManager == nullptr)
		return;
	
	for (int32 i = 0; i < SpawnDescs[CurrentAction].SpawnCountPerOnce; ++i)
	{
		ARPGProjectileActor* Projectile = Cast<ARPGProjectileActor>(PoolManager->GetPoolObject(ObjectPoolKey));
		if (Projectile == nullptr)
			return;

		FTransform SpawnTransform = GetSpawnTransform(i);
		Projectile->SetActorTransform(SpawnTransform);

		SettingProjectile(Projectile, SpawnTransform);
	}

	CurrentAction++;
}


void URPGAbility_SpawnProjectile::SettingProjectile_Implementation(ARPGProjectileActor* Projectile, const FTransform& SpawnTransform)
{
	TArray<FGameplayEffectSpecHandle> EffectSpecHandles;
	GetEffectSpecHandles(EffectSpecHandles);
	Projectile->SetEffectSpecHandles(EffectSpecHandles);
	
	Projectile->SetActorTransform(SpawnTransform);
	Projectile->ResetVelocity();
}




FTransform URPGAbility_SpawnProjectile::GetSpawnTransform(const int32 CurSpawnCount) const
{
	FTransform SpawnTransform;

	ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningActorFromActorInfo());
	if (OwningCharacter == nullptr)
		return SpawnTransform;
	
	FRotator PivotSpawnRotator = OwningCharacter->GetActorForwardVector().Rotation();
	PivotSpawnRotator.Yaw += SpawnDescs[CurrentAction].SpawnRotationPivotAngle + SpawnDescs[CurrentAction].SpreadAngle * CurSpawnCount;
	SpawnTransform.SetRotation(PivotSpawnRotator.Quaternion());

	FVector WorldOffset = SpawnTransform.TransformPosition(SpawnDescs[CurrentAction].SpawnOffset);
	FVector SpawnLocation = OwningCharacter->GetMesh()->GetSocketLocation(SpawnDescs[CurrentAction].SpawnSocketName) + WorldOffset;
	SpawnTransform.SetLocation(SpawnLocation);
	
	return SpawnTransform;
}
