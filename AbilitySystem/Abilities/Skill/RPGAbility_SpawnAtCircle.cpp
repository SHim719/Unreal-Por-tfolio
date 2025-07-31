// 


#include "RPGAbility_SpawnAtCircle.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Actor/RPGTargetCircle.h"
#include "Actor/EffectActor/RPGSphereAreaEffectActor.h"
#include "Kismet/GameplayStatics.h"
#include "Player/RPGPlayerController.h"

void URPGAbility_SpawnAtCircle::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                                const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                                const FGameplayEventData* TriggerEventData)
{
	if (CheckCost(Handle, ActorInfo) == false || CheckCooldown(Handle, ActorInfo) == false)
		return;
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


	if (auto* PC = Cast<ARPGPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
	{
		ARPGTargetCircle* TargetCircle = PC->ShowTargetCircle(RangeRadius);
		TargetCircle->OnConfirmedPosition.BindUObject(this, &URPGAbility_SpawnAtCircle::SpawnActorAtPosition);
	}
	
}


void URPGAbility_SpawnAtCircle::InputPressed(const FGameplayAbilitySpecHandle Handle,
                                             const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (IsActive())
	{
		if (auto* PC = Cast<ARPGPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			PC->HideTargetCircle();
		}

		GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			K2_EndAbility();
		});
	}
}

void URPGAbility_SpawnAtCircle::SpawnActorAtPosition(bool bCanceled, const FVector& SpawnPosition)
{
	if (bCanceled == false && CommitAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo()))
	{
		if (SpawnActorClass)
		{
			FTransform SpawnTransform;
			SpawnTransform.SetLocation(SpawnPosition);
			ARPGSphereAreaEffectActor* SpawnedActor = GetWorld()->SpawnActorDeferred<ARPGSphereAreaEffectActor>(SpawnActorClass, SpawnTransform, GetAvatarActorFromActorInfo(), Cast<APawn>(GetAvatarActorFromActorInfo()));
			SpawnedActor->SetLifeSpan(GetAttributeDataFromCurveTableByTag(DurationTag));

			TArray<FGameplayEffectSpecHandle> EffectSpecHandles;
			GetEffectSpecHandles(EffectSpecHandles);
			
			SpawnedActor->SetEffectSpecHandles(EffectSpecHandles);
			SpawnedActor->FinishSpawning(SpawnTransform);
		}
		
		if (auto* PC = Cast<ARPGPlayerController>(UGameplayStatics::GetPlayerController(this, 0)))
		{
			PC->HideTargetCircle();
		}
	}

	K2_EndAbility();
}

