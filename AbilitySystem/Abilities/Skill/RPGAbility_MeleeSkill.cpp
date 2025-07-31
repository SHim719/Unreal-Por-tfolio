#include "AbilitySystem/Abilities/Skill/RPGAbility_MeleeSkill.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "RPGHelperLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

URPGAbility_MeleeSkill::URPGAbility_MeleeSkill()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void URPGAbility_MeleeSkill::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
		return;
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	QueryDamageEventTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
				this, FGameplayTag::RequestGameplayTag(FName("Combat.QueryDamage")), nullptr, false, false);
	if (QueryDamageEventTask)
	{
		QueryDamageEventTask->EventReceived.AddDynamic(this, &ThisClass::QueryDamage);
		QueryDamageEventTask->ReadyForActivation();
	}
	
	if (bInputCombo)
	{
		bCanNextCombo = false;
		WaitEnableNextSkillTask = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, FGameplayTag::RequestGameplayTag(FName("Combat.EnableNextCombo")), nullptr, false, true);
		if (WaitEnableNextSkillTask)
		{
			WaitEnableNextSkillTask->EventReceived.AddDynamic(this, &ThisClass::SetEnableNextCombo);
			WaitEnableNextSkillTask->ReadyForActivation();
		}
	}

	CurComboCount = 0;
	
}

void URPGAbility_MeleeSkill::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void URPGAbility_MeleeSkill::InputPressed(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
	if (bCanNextCombo == false || CurComboCount >= MaxComboCount)
		return;

	bCanNextCombo = false;
	CurComboCount = (CurComboCount + 1) % MaxComboCount;
	
	const FString SectionName = FString::Printf(TEXT("Attack%d"), CurComboCount);
	MontageJumpToSection(FName(SectionName));
	
}

float URPGAbility_MeleeSkill::GetAttributeDataFromCurveTableByTag(const FGameplayTag& Tag) const
{
	if (AbilityAttributeTable == nullptr)
		return 0.f;
	
	const FString QueryString = Tag.ToString() + FString::Printf(TEXT(".%d"), CurComboCount);
	const FName RowName = FName(QueryString);

	if (const FRealCurve* DamageDataCurve = AbilityAttributeTable->FindCurve(RowName, "QueryDamageData"))
	{
		const float Value = DamageDataCurve->Eval(GetAbilityLevel());
		return Value;
	}

	return 0.f;
}


void URPGAbility_MeleeSkill::SetEnableNextCombo(FGameplayEventData EventData)
{
	if (EventData.EventTag.MatchesTagExact(FGameplayTag::RequestGameplayTag(FName("Combat.EnableNextCombo"))))
	{
		bCanNextCombo = true;
	}
	else
	{
		K2_EndAbility();
	}
}

void URPGAbility_MeleeSkill::QueryDamage_Implementation(FGameplayEventData EventData)
{
	const float AttackRange = HitQueryDefines[CurComboCount].AttackRange;
	
	AActor* Instigator = GetAvatarActorFromActorInfo();
	TArray<FHitResult> HitResults;
	const FVector Start = Instigator->GetActorLocation();
	FCollisionShape CollisionShape = FCollisionShape::MakeSphere(AttackRange); 
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(Instigator);
	
	bool bHit = GetWorld()->SweepMultiByObjectType(
		HitResults,
		Start,
		Start,
		FQuat::Identity,
		ECC_Pawn,      
		CollisionShape,
		QueryParams
	);

	if (bHit)
	{
		TArray<FGameplayEffectSpecHandle> EffectSpecHandles;
		GetEffectSpecHandles(EffectSpecHandles);	
		
		const float AttackAngle = HitQueryDefines[CurComboCount].AttackAngle;
		
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			FVector ToHitDirection = (HitActor->GetActorLocation() - Start).GetSafeNormal2D();
			const float HitAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Instigator->GetActorForwardVector().GetSafeNormal2D(), ToHitDirection)));
			if (HitAngle < AttackAngle)
			{
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
	
	if (bInputCombo == false)
	{
		CurComboCount = (CurComboCount + 1) % MaxComboCount;
	}
}

