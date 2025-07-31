#include "RPGAbility_MutantGroundSmash.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "AbilitySystem/Task/AbilityTask_Tick.h"
#include "Actor/Indicator/RPGRadialIndicator.h"
#include "Engine/OverlapResult.h"

void URPGAbility_MutantGroundSmash::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                    const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                    const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;

	WaitHitEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, FGameplayTag::RequestGameplayTag(FName("Combat.QueryDamage")), nullptr, false, true);
	if (WaitHitEventTask)
	{
		WaitHitEventTask->EventReceived.AddDynamic(this, &ThisClass::SmashAttack);
		WaitHitEventTask->ReadyForActivation();
	}
	

	TickTask = UAbilityTask_Tick::CreateAbilityTickTask(this);
	if (TickTask)
	{
		TickTask->TickDelegate.AddUObject(this, &URPGAbility_MutantGroundSmash::WaitSmashAttack);
		TickTask->ReadyForActivation();
	}

	SettingWarningIndicator();

	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
}

void URPGAbility_MutantGroundSmash::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	CurrentTime = 0.f;
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGAbility_MutantGroundSmash::SettingWarningIndicator()
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	
	FVector SpawnLocation = RPGHelper::GetActorFootLocation(OwnerActor) + OwnerActor->GetActorForwardVector() * HitQueryLocalOffset;
	FRotator SpawnRotation = OwnerActor->GetActorRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = Cast<APawn>(GetOwningActorFromActorInfo());
	
	WarningIndicator = Cast<ARPGRadialIndicator>(GetWorld()->SpawnActor(IndicatorClass, &SpawnLocation, &SpawnRotation, SpawnParams));
	WarningIndicator->Initialize(AttackRange, 360.f, 0.f);
	
}

void URPGAbility_MutantGroundSmash::WaitSmashAttack(float DeltaTime)
{
	CurrentTime += DeltaTime;
	if (CurrentTime >= SmashWaitTime)
	{
		TickTask->TickDelegate.Clear();

		WarningIndicator->Destroy();
		WarningIndicator = nullptr;
	}
	else
	{
		WarningIndicator->UpdateFillAmount(CurrentTime / SmashWaitTime);
	}
	
}

void URPGAbility_MutantGroundSmash::SmashAttack(FGameplayEventData EventData)
{
	AActor* OwnerActor = GetOwningActorFromActorInfo();
	FVector QueryLocation = RPGHelper::GetActorFootLocation(OwnerActor) + OwnerActor->GetActorForwardVector() * HitQueryLocalOffset;
	
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwningActorFromActorInfo());
	
	bool bResult = GetWorld()->OverlapMultiByObjectType(OverlapResults, QueryLocation, FQuat::Identity,ECC_Pawn,      
		FCollisionShape::MakeSphere(AttackRange),
		QueryParams);

	if (bResult)
	{
		TArray<FGameplayEffectSpecHandle> EffectSpecHandles;
		GetEffectSpecHandles(EffectSpecHandles);
		
		for (FOverlapResult& Result : OverlapResults)
		{
			AActor* HitActor = Result.GetActor();
			
			if (RPGHelper::IsFriend(GetOwningActorFromActorInfo(), HitActor) == true)
				continue;
				
			if (UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(HitActor))
			{
				for (const FGameplayEffectSpecHandle& EffectSpecHandle : EffectSpecHandles)
				{
					if (EffectSpecHandle.IsValid())
					{
						TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
					}
				}
			}
		}
	}
}


