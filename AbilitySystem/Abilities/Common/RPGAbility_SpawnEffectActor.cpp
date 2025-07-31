// 


#include "RPGAbility_SpawnEffectActor.h"

#include "RPGHelperLibrary.h"
#include "AbilitySystem/RPGAbilitySystemComponent.h"
#include "Actor/EffectActor/RPGEffectActor.h"
#include "ObjectPool/RPGPoolManager.h"

void URPGAbility_SpawnEffectActor::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
                                                 const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	if (Spec.Level >= 1)
	{
		InitObjectPool();
	}
	else 
	{
		if (auto* ASC = Cast<URPGAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
		{
			ASC->OnAbilityLevelChanged.AddUObject(this, &ThisClass::OnChangeAbilityLevel);
		}
	}
}

void URPGAbility_SpawnEffectActor::InitObjectPool()
{
	if (InitPoolSize > 0 && EffectActorClassToSpawn && ObjectPoolKey.IsEmpty() == false)
	{
		if (URPGPoolManager* PoolManager = RPGHelper::GetPoolManager(this))
		{
			PoolManager->CreateNewPool(ObjectPoolKey, EffectActorClassToSpawn, InitPoolSize, InitPoolSize, true);
		}
	}
}

void URPGAbility_SpawnEffectActor::OnChangeAbilityLevel(const FGameplayTag& Tag, const int32 NewLevel)
{
	if (NewLevel >= 1)
	{
		if (auto* Spec = GetCurrentAbilitySpec())
		{
			if (Spec->Ability->GetAssetTags().HasTagExact(Tag))
			{
				InitObjectPool();

				// 오브젝트 풀이 생성되면 더이상 AbilityLevelChanged의 소식을 듣지않음.
				if (auto* ASC = Cast<URPGAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
				{
					ASC->OnAbilityLevelChanged.RemoveAll(this);
				}
			}
		}
	}
}
